#define TINYGLTF_NOEXCEPTION
#include <tiny_gltf.h>
#include <vulkan/vulkan.h>
#include <algorithm>
#include <functional>
#include <meshoptimizer.h>
#include <limits>
#include <flecs.h>
#include <DirectXMath.h>
#undef assert

module crude.resources.gltf_model_loader_system;

import crude.gfx.renderer_deferred_gbuffer_pbr_pass_system;
import crude.gfx.renderer_deferred_gbuffer_color_pass_system;
import crude.gfx.vk.device;
import crude.gfx.vk.sampler;
import crude.gfx.vk.command_buffer;
import crude.gfx.vk.generate_mipmaps;
import crude.gfx.vk.command_pool;
import crude.gfx.vk.flush;
import crude.gfx.vk.image;
import crude.gfx.vk.image_2d;
import crude.gfx.vk.image_view;
import crude.gfx.texture;
import crude.gfx.material;
import crude.gfx.mesh_buffer;
import crude.scene.transform;
import crude.core.logger;
import crude.core.assert;

namespace crude::resources
{

flecs::system registerGLTFModelLoaderSystem(flecs::world world, core::shared_ptr<GLTF_Model_Loader_Context> ctx)
{
  return world.system<GLTF_Model_Metadata_Component>("GLTFModelLoaderSystem")
    .ctx(ctx.get())
    .kind(flecs::OnLoad)
    .with<GLTF_Model_Loader_Uninitialized_Flag>()
    .run(gltfModelLoaderSystemProcess);
}

// 0 - crude::resources::GLTF_Model_Metadata_Component
// 1 - crude::resources::GLTF_Model_Loader_Uninitialized_Flag
void gltfModelLoaderSystemProcess(flecs::iter & it)
{
  GLTF_Model_Loader_Context* ctx = it.ctx<GLTF_Model_Loader_Context>();
  while (it.next())
  {
    auto metadata = it.field<crude::resources::GLTF_Model_Metadata_Component>(0);

    for (auto i : it)
    {
      flecs::entity entity = it.entity(i);

      GLTF_Loader gltfLoader(ctx->transferCommandPool);
      gltfLoader.loadToNodeFromFile(entity, metadata[i].path.c_str());
      ctx->callback(entity);
      entity.remove<crude::resources::GLTF_Model_Loader_Uninitialized_Flag>();
    }
  }
}

GLTF_Loader::GLTF_Loader(core::shared_ptr<gfx::vk::Command_Pool> commandPool)
  : m_commandPool(commandPool)
{}

void GLTF_Loader::loadToNodeFromFile(flecs::entity node, const std::filesystem::path& path)
{
  m_imageViews.clear();
  m_textures.clear();
  m_meshes.clear();
  m_meshBuffers.clear();
  m_pointLights.clear();

  if (!loadModelFromFile(path))
    return;

  m_imageViews.resize(m_tinyModel.images.size() + 1);
  m_textures.resize(m_tinyModel.textures.size() + 1);

  m_sampler = core::allocateShared<gfx::vk::Sampler>(m_commandPool->getDevice(), gfx::vk::csamlper_state::gMagMinMipLinearRepeat);
  
  // Load meshes
  m_meshes.reserve(m_tinyModel.meshes.size());
  
  for (const tinygltf::Mesh& tinyMesh : m_tinyModel.meshes)
  {
    core::shared_ptr<scene::Mesh> mesh = core::allocateShared<scene::Mesh>();

    for (const tinygltf::Primitive& tinyPrimitive : tinyMesh.primitives)
    {
      const core::uint32 verticesOffset = mesh->vertices.size();
      const core::uint32 primitiviesOffset = mesh->primitiveIndices.size();
      const core::uint32 vertexIndicestOffset = mesh->vertexIndices.size();
      const core::uint32 meshletsOffset = mesh->meshlets.size();

      core::vector<scene::Vertex> primtiveVertices = loadVerticesFromPrimitive(tinyPrimitive);
      core::vector<core::uint32> primitiveVertexIndices = loadVertexIndicesFromPrimitive(tinyPrimitive);

      buildMeshlets(primtiveVertices, primitiveVertexIndices, mesh->vertexIndices, mesh->primitiveIndices, mesh->meshlets);
      mesh->vertices.insert(mesh->vertices.end(), primtiveVertices.begin(), primtiveVertices.end());

      core::shared_ptr<gfx::Material> submeshMaterial = parseMaterial(tinyPrimitive.material);
      
      mesh->submeshes.push_back(scene::Sub_Mesh{
        .vertexOffset = verticesOffset,
        .vertexCount = static_cast<core::uint32>(primtiveVertices.size()),
        .lodCount = 1u,
        .lods = core::array<scene::Sub_Mesh_Lod, 8> { scene::Sub_Mesh_Lod {
          .indexOffset = vertexIndicestOffset,
          .indexCount = static_cast<core::uint32>(mesh->vertexIndices.size() - vertexIndicestOffset),
          .meshletOffset = meshletsOffset,
          .meshletCount = static_cast<core::uint32>(mesh->meshlets.size() - meshletsOffset),
        } },
        .material = submeshMaterial
        });
    }

    m_meshes.push_back(mesh);
  }

   m_meshBuffers.reserve(m_meshes.size());
  for (core::shared_ptr<scene::Mesh> mesh : m_meshes)
  {
    auto commandBuffer = core::allocateShared<gfx::vk::Command_Buffer>(m_commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    m_meshBuffers.push_back(core::allocateShared<gfx::Mesh_Buffer>(commandBuffer, *mesh));
  }

  // Load lights
  for (const tinygltf::Light& tinyLight : m_tinyModel.lights)
  {
    if (tinyLight.type == "point")
    {
      m_pointLights.push_back(parsePointLight(tinyLight));
    }
    else
    {
      m_pointLights.push_back(scene::Point_Light_CPU{});
    }
  }

  // Load nodes
  for (const tinygltf::Node& tinyNode : m_tinyModel.nodes)
  {
    flecs::entity child = parseNode(node.world(), tinyNode);
    child.child_of(node);
  }
  
  if (!node.has<scene::Transform>())
    node.set<scene::Transform>(scene::Transform(node));
}

core::shared_ptr<gfx::vk::Image_View> GLTF_Loader::parseImageView(const core::int32 tinyImageIndex, VkFormat format, VkFilter mipmapFilter)
{
  if (m_imageViews[tinyImageIndex])
  {
    return m_imageViews[tinyImageIndex];
  }

  const tinygltf::Image& tinyImage = m_tinyModel.images[tinyImageIndex];

  auto commandBuffer = core::allocateShared<gfx::vk::Command_Buffer>(m_commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);

  core::shared_ptr<gfx::vk::Image> image;

  // !TODO
  if (tinyImage.width > cTextureExtentLimit.width || tinyImage.height > cTextureExtentLimit.height)
  {
    VkExtent3D originalExtent;
    originalExtent.width  = tinyImage.width;
    originalExtent.height = tinyImage.height;
    originalExtent.depth  = 1u;

    core::shared_ptr<gfx::vk::Image> originalImage = core::allocateShared<gfx::vk::Image_2D>(
      commandBuffer,
      format,
      gfx::vk::Image::Mip_Data(originalExtent, core::span<const core::byte>(tinyImage.image)),
      1,
      VK_SHARING_MODE_EXCLUSIVE);

    VkExtent3D limitedExtent;
    limitedExtent.width  = std::min<core::uint32>(originalExtent.width, cTextureExtentLimit.width);
    limitedExtent.height = std::min<core::uint32>(originalExtent.height, cTextureExtentLimit.height);
    limitedExtent.depth  = 1u;

    core::shared_ptr<gfx::vk::Image> resizedImage = core::allocateShared<gfx::vk::Image_2D>(
      commandBuffer->getDevice(),
      format,
      limitedExtent,
      scene::calculateMaximumMipLevelsCount(limitedExtent.width, limitedExtent.height),
      VK_SHARING_MODE_EXCLUSIVE);

    commandBuffer->begin();
    commandBuffer->barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
      gfx::vk::Image_Memory_Barrier(originalImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, gfx::vk::Image_Subresource_Range(originalImage, 0u, 1u, 0u, originalImage->getArrayLayersCount())));
    commandBuffer->barrier(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
      gfx::vk::Image_Memory_Barrier(resizedImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, gfx::vk::Image_Subresource_Range(resizedImage)));
    
    commandBuffer->blitImage(gfx::vk::Image_Blit_Region(originalImage, resizedImage), VK_FILTER_LINEAR);
    
    commandBuffer->barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, 
      gfx::vk::Image_Memory_Barrier(resizedImage, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, gfx::vk::Image_Subresource_Range(resizedImage)));
    commandBuffer->end();
    gfx::vk::flush(commandBuffer);
  
    image = resizedImage;
  }
  else
  {
    VkExtent3D originalExtent;
    originalExtent.width  = tinyImage.width;
    originalExtent.height = tinyImage.height;
    originalExtent.depth  = 1u;

    core::shared_ptr<gfx::vk::Image> originalImage = core::allocateShared<gfx::vk::Image_2D>(
      commandBuffer,
      format,
      gfx::vk::Image::Mip_Data(originalExtent, core::span<const core::byte>(tinyImage.image)),
      1,
      VK_SHARING_MODE_EXCLUSIVE);

    image = originalImage;
  }
  
  commandBuffer->begin();
  gfx::vk::generateMipmaps(commandBuffer, image, mipmapFilter);
  commandBuffer->end();
  gfx::vk::flush(commandBuffer);

  m_imageViews[tinyImageIndex] = core::allocateShared<gfx::vk::Image_View>(image);
  return m_imageViews[tinyImageIndex];
}

core::shared_ptr<gfx::Texture> GLTF_Loader::parseTexture(const core::int32 tinyTextureIndex, const VkFormat format, core::span<const core::byte> texelForUnitialized)
{
  if (m_textures[tinyTextureIndex + 1])
  {
    return m_textures[tinyTextureIndex + 1];
  }

  if (tinyTextureIndex == -1)
  {
    auto commandBuffer = core::allocateShared<gfx::vk::Command_Buffer>(m_commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    core::shared_ptr<gfx::vk::Image> image = core::allocateShared<gfx::vk::Image_2D>(
      commandBuffer,
      format,
      gfx::vk::Image::Mip_Data(VkExtent3D{ 1, 1, 1 }, texelForUnitialized),
      VK_SHARING_MODE_EXCLUSIVE);

    m_textures[tinyTextureIndex + 1] = core::allocateShared<gfx::Texture>(core::allocateShared<gfx::vk::Image_View>(image), m_sampler);
  }
  else
  {
    auto imageView = parseImageView(m_tinyModel.textures[tinyTextureIndex].source, format, VK_FILTER_LINEAR);
    m_textures[tinyTextureIndex + 1] = core::allocateShared<gfx::Texture>(imageView, m_sampler);
  }

  return m_textures[tinyTextureIndex + 1];
}

scene::Point_Light_CPU GLTF_Loader::parsePointLight(const tinygltf::Light& tinyLight)
{
  scene::Point_Light_CPU pointLight;
  const DirectX::XMVECTOR color = DirectX::XMVectorSet(
    tinyLight.color.size() > 0 ? tinyLight.color[0] : 0,
    tinyLight.color.size() > 1 ? tinyLight.color[1] : 0,
    tinyLight.color.size() > 2 ? tinyLight.color[2] : 0,
    tinyLight.color.size() > 3 ? tinyLight.color[3] : 0);
  DirectX::XMStoreFloat3(&pointLight.intensity, color);
  pointLight.sourceRadius = 0.3;
  return pointLight;
}

core::shared_ptr<gfx::Material> GLTF_Loader::parseMaterial(core::int32 tinyMaterialIndex)
{
  if (tinyMaterialIndex == -1)
  {
    return nullptr;
  }

  const tinygltf::Material& tinyMaterial = m_tinyModel.materials[tinyMaterialIndex];
  core::shared_ptr<gfx::Material> material = core::allocateShared<gfx::Material>();

  //core::assert(tinyMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index != -1 && tinyMaterial.normalTexture.index != -1 && tinyMaterial.pbrMetallicRoughness.baseColorTexture.index != -1);

  // !TODO :D
  if (tinyMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
  {
    if (tinyMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0 && tinyMaterial.normalTexture.index >= 0)
    {
      material->albedo = parseTexture(tinyMaterial.pbrMetallicRoughness.baseColorTexture.index, VK_FORMAT_R8G8B8A8_SRGB, core::array<core::byte, 4> { 255, 255, 255, 255 });
      material->metallicRoughness = parseTexture(tinyMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index, VK_FORMAT_R8G8B8A8_SRGB, core::array<core::byte, 4>{0, 0, 0, 0});
      material->normal = parseTexture(tinyMaterial.normalTexture.index, VK_FORMAT_R8G8B8A8_SRGB, core::array<core::byte, 4> { 0, 0, 0, 0 });
    }
    else
    {
      material->albedo = parseTexture(tinyMaterial.pbrMetallicRoughness.baseColorTexture.index, VK_FORMAT_R8G8B8A8_SRGB, core::array<core::byte, 4> { 255, 255, 255, 255 });
    }
  }
  return material;
}

bool GLTF_Loader::loadModelFromFile(const std::filesystem::path& path)
{
  tinygltf::TinyGLTF tinyLoader;
  std::string err;
  std::string warn;

  bool loadResult = false;
  
  if (path.extension() == ".glb")
  {
    loadResult = tinyLoader.LoadBinaryFromFile(&m_tinyModel, &err, &warn, path.string());
  }
  else if (path.extension() == ".gltf")
  {
    loadResult = tinyLoader.LoadASCIIFromFile(&m_tinyModel, &err, &warn, path.string());
  }

  if (!warn.empty())
  {
    core::logWarning(core::Debug::Channel::FileIO, "Warning while loading model geometry: %s", warn.c_str());
  }

  if (!err.empty())
  {
    core::logError(core::Debug::Channel::FileIO, "Error while loading model geometry: %s", err.c_str());
    return false;
  }

  if (!loadResult)
  {
    core::logError(core::Debug::Channel::FileIO, "Failed to parse glTF: %s", path.c_str());
    return false;
  }

  return true;
}


flecs::entity GLTF_Loader::parseNode(flecs::world world, const tinygltf::Node& tinyNode)
{
  static core::uint32 uniqueNodeID = 0u;
  flecs::entity node(world, tinyNode.name.empty() ? ("Node " + std::to_string(uniqueNodeID++)).c_str() : tinyNode.name.c_str());

  if (tinyNode.extensions.contains("CRUDE_node_deferred_color"))
  {
    node.add<gfx::Deferred_Node_Pipeline_Color_Flag>();
  }
  else
  {
    node.add<gfx::Deferred_Node_Pipeline_PBR_Flag>();
  }

  if (tinyNode.light != -1)
  {
    node.set<scene::Point_Light_CPU>(m_pointLights[tinyNode.light]);
  }

  scene::Transform transform(node);
  if (tinyNode.matrix.size() > 0)
  {
    transform.setNodeToParent(DirectX::XMMATRIX(
      tinyNode.matrix[0], tinyNode.matrix[1], tinyNode.matrix[2], tinyNode.matrix[3],
      tinyNode.matrix[4], tinyNode.matrix[5], tinyNode.matrix[6], tinyNode.matrix[7],
      tinyNode.matrix[8], tinyNode.matrix[9], tinyNode.matrix[10], tinyNode.matrix[11],
      tinyNode.matrix[12], tinyNode.matrix[13], tinyNode.matrix[14], tinyNode.matrix[15]));
  }
  else
  {
    if (!tinyNode.translation.empty())
      transform.setTranslation(tinyNode.translation[0], tinyNode.translation[1], tinyNode.translation[2]);
    if (!tinyNode.rotation.empty())
      transform.setRotationQuaternion(tinyNode.rotation[0], tinyNode.rotation[1], tinyNode.rotation[2], tinyNode.rotation[3]);
    if (!tinyNode.scale.empty())
      transform.setScale(tinyNode.scale[0], tinyNode.scale[1], tinyNode.scale[2]);
  }
  node.set<scene::Transform>(std::move(transform));

  if (tinyNode.mesh != -1)
  {
    node.set<core::shared_ptr<scene::Mesh>>(m_meshes[tinyNode.mesh]);
    node.set<core::shared_ptr<gfx::Mesh_Buffer>>(m_meshBuffers[tinyNode.mesh]);
  }

  for (core::uint32 childIndex : tinyNode.children)
  {
    flecs::entity child = parseNode(world, m_tinyModel.nodes[childIndex]);
    child.child_of(node);
  }
  return node;
}

core::vector<scene::Vertex> GLTF_Loader::loadVerticesFromPrimitive(const tinygltf::Primitive& tinyPrimitive)
{
  const core::uint32 primitiveVerticesCount = m_tinyModel.accessors[tinyPrimitive.attributes.begin()->second].count;
  core::vector<scene::Vertex> primitiveVertices(primitiveVerticesCount);

  for (const auto& attribute : tinyPrimitive.attributes)
  {
    const tinygltf::Accessor& tinyAccessor = m_tinyModel.accessors[attribute.second];
    if (attribute.first == "POSITION")
    {
      core::assert(tinyAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
      core::assert(tinyAccessor.type == TINYGLTF_TYPE_VEC3);
      loadBufferFromAccessor(tinyAccessor, reinterpret_cast<core::byte*>(&primitiveVertices[0].position.x), sizeof(DirectX::XMFLOAT3), sizeof(primitiveVertices[0]));
    }
    else if (attribute.first == "TEXCOORD_0")
    {
      core::assert(tinyAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
      core::assert(tinyAccessor.type == TINYGLTF_TYPE_VEC2);
      loadBufferFromAccessor(tinyAccessor, reinterpret_cast<core::byte*>(&primitiveVertices[0].texcoord.x), sizeof(DirectX::XMFLOAT2), sizeof(primitiveVertices[0]));
    }
    else if (attribute.first == "NORMAL")
    {
      core::assert(tinyAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
      core::assert(tinyAccessor.type == TINYGLTF_TYPE_VEC3);
      loadBufferFromAccessor(tinyAccessor, reinterpret_cast<core::byte*>(&primitiveVertices[0].normal.x), sizeof(DirectX::XMFLOAT3), sizeof(primitiveVertices[0]));
    }
    else if (attribute.first == "COLOR_0")
    {
      core::assert(tinyAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
      core::assert(tinyAccessor.type == TINYGLTF_TYPE_VEC4);
      loadBufferFromAccessor(tinyAccessor, reinterpret_cast<core::byte*>(&primitiveVertices[0].color.x), sizeof(DirectX::XMFLOAT4), sizeof(primitiveVertices[0]));
    }
  }

  return primitiveVertices;
}

core::vector<core::uint32> GLTF_Loader::loadVertexIndicesFromPrimitive(const tinygltf::Primitive& tinyPrimitive)
{
  const tinygltf::Accessor& tinyAccessor = m_tinyModel.accessors[tinyPrimitive.indices];
  const tinygltf::BufferView& tinyBufferView = m_tinyModel.bufferViews[tinyAccessor.bufferView];
  const tinygltf::Buffer& tinyBuffer = m_tinyModel.buffers[tinyBufferView.buffer];

  core::vector<core::uint32> indices(tinyAccessor.count);

  core::assert(tinyAccessor.type == TINYGLTF_TYPE_SCALAR);

  const core::byte* bufferData = reinterpret_cast<const core::byte*>(&tinyBuffer.data[tinyAccessor.byteOffset + tinyBufferView.byteOffset]);
  for (core::size_t i = 0; i < tinyAccessor.count; ++i)
  {
    if (tinyAccessor.componentType == TINYGLTF_COMPONENT_TYPE_SHORT)
    {
      const core::int16* tinyIndices = reinterpret_cast<const core::int16*>(bufferData);
      indices[i] = tinyIndices[i];
    }
    else if (tinyAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
    {
      const core::uint16* tinyIndices = reinterpret_cast<const core::uint16*>(bufferData);
      indices[i] = tinyIndices[i];
    }
    else
    {
      core::assert(false);
    }
    bufferData += tinyBufferView.byteStride;
  }
  return indices;
}

void GLTF_Loader::loadBufferFromAccessor(const tinygltf::Accessor& tinyAccessor, core::byte* data, core::size_t elementSize, core::size_t byteStride)
{
  const tinygltf::BufferView& tinyBufferView = m_tinyModel.bufferViews[tinyAccessor.bufferView];
  const tinygltf::Buffer& tinyBuffer = m_tinyModel.buffers[tinyBufferView.buffer];

  const core::byte* bufferData = reinterpret_cast<const core::byte*>(&tinyBuffer.data[tinyAccessor.byteOffset + tinyBufferView.byteOffset]);
  for (core::size_t i = 0u; i < tinyAccessor.count; ++i)
  {
    std::memcpy(data, bufferData, elementSize);
    data += byteStride;
    bufferData += tinyBufferView.byteStride;
  }
}

void GLTF_Loader::buildMeshlets(const core::vector<scene::Vertex>&  submeshVertices,
                                const core::vector<core::uint32>&   submeshVertexIndices,
                                core::vector<core::uint32>&         meshVertexIndices,
                                core::vector<core::uint8>&          meshPrimitiveIndices,
                                core::vector<scene::Meshlet>&       meshMeshlets)
{
  constexpr core::size_t maxVertices = 64u;
  constexpr core::size_t maxTriangles = 124u;
  constexpr core::float32 coneWeight = 0.0f;

  meshopt_setAllocator(core::defaultAllocate, core::defaultDeallocate);

  const core::uint32 meshVertexIndicesOffset = meshVertexIndices.size();
  const core::uint32 meshPrimitiveIndicesOffset = meshPrimitiveIndices.size();
  const core::uint32 meshMeshletOffset = meshMeshlets.size();

  const core::size_t maxMeshlets = meshopt_buildMeshletsBound(submeshVertexIndices.size(), maxVertices, maxTriangles);

  core::vector<meshopt_Meshlet> zeuxMeshlets(maxMeshlets);
  meshVertexIndices.resize(meshVertexIndicesOffset + maxMeshlets * maxVertices);
  meshPrimitiveIndices.resize(meshPrimitiveIndicesOffset + maxMeshlets * maxTriangles * 3);

  const core::size_t meshletCount = meshopt_buildMeshlets(zeuxMeshlets.data(), 
    &meshVertexIndices[meshVertexIndicesOffset], &meshPrimitiveIndices[meshPrimitiveIndicesOffset], 
    submeshVertexIndices.data(), submeshVertexIndices.size(), 
    &submeshVertices[0].position.x, submeshVertices.size(), sizeof(scene::Vertex), 
    maxVertices, maxTriangles, coneWeight);

  const meshopt_Meshlet& zeuxLastMeshlet = zeuxMeshlets[meshletCount - 1];
  meshVertexIndices.resize(meshVertexIndicesOffset + zeuxLastMeshlet.vertex_offset + zeuxLastMeshlet.vertex_count);
  meshPrimitiveIndices.resize(meshPrimitiveIndicesOffset + zeuxLastMeshlet.triangle_offset + ((zeuxLastMeshlet.triangle_count * 3 + 3) & ~3));
  zeuxMeshlets.resize(meshletCount);

  meshMeshlets.reserve(meshMeshletOffset + meshletCount);

  for (const meshopt_Meshlet& zeuxMeshlet : zeuxMeshlets)
  {
    core::assert(zeuxMeshlet.vertex_count <= std::numeric_limits<core::uint8>::max());
    core::assert(zeuxMeshlet.triangle_count <= std::numeric_limits<core::uint8>::max());

    meshopt_optimizeMeshlet(
      &meshVertexIndices[meshVertexIndicesOffset + zeuxMeshlet.vertex_offset],
      &meshPrimitiveIndices[meshPrimitiveIndicesOffset + zeuxMeshlet.triangle_offset],
      zeuxMeshlet.triangle_count,
      zeuxMeshlet.vertex_count);

    meshMeshlets.push_back(scene::Meshlet{
      .vertexCount = static_cast<core::uint8>(zeuxMeshlet.vertex_count),
      .primitiveCount = static_cast<core::uint8>(zeuxMeshlet.triangle_count),
      .vertexOffset = meshVertexIndicesOffset + zeuxMeshlet.vertex_offset,
      .primitiveOffest = meshPrimitiveIndicesOffset + zeuxMeshlet.triangle_offset,
      });
  }
}

}