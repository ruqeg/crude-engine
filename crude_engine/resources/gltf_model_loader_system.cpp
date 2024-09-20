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

import crude.graphics.device;
import crude.graphics.sampler;
import crude.graphics.command_buffer;
import crude.graphics.generate_mipmaps;
import crude.graphics.command_pool;
import crude.graphics.flush;
import crude.graphics.image_2d;
import crude.graphics.image_view;
import crude.graphics.texture;
import crude.graphics.material;
import crude.graphics.mesh_buffer;
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
      loadModelToNodeFromFile(it.entity(i), metadata[i].path.c_str(), ctx->transferCommandPool);
      it.entity(i).remove<crude::resources::GLTF_Model_Loader_Uninitialized_Flag>();
    }
  }
}

void loadModelToNodeFromFile(flecs::entity node, const char* path, core::shared_ptr<graphics::Command_Pool> commandPool)
{
  tinygltf::Model tinyModel;
  
  if (!loadModelFromFile(tinyModel, path))
    return;

  // Load samples
  core::vector<core::shared_ptr<graphics::Sampler>> samplers;
  samplers.reserve(tinyModel.samplers.size());
  for (const tinygltf::Sampler& tinySampler : tinyModel.samplers)
  {
    samplers.push_back(parseSampler(commandPool->getDevice(), tinySampler));
  }

  // Load images
  core::vector<core::shared_ptr<graphics::Image>> images;
  images.reserve(tinyModel.images.size());
  for (const tinygltf::Image& tinyImage : tinyModel.images)
  {
    images.push_back(parseImage(commandPool, tinyImage));
  }

  // Load textures
  core::vector<core::shared_ptr<graphics::Texture>> textures;
  textures.reserve(tinyModel.textures.size());
  for (const tinygltf::Texture& tinyTexture : tinyModel.textures)
  {
    core::shared_ptr<graphics::Image_View> imageView = nullptr;
    core::shared_ptr<graphics::Sampler> sampler = nullptr;

    if (tinyTexture.source >= 0 && tinyTexture.source < images.size())
      imageView = core::allocateShared<graphics::Image_View>(images[tinyTexture.source], graphics::Image_Subresource_Range(images[tinyTexture.source]));
    if (tinyTexture.sampler >= 0 && tinyTexture.sampler < samplers.size())
      sampler = samplers[tinyTexture.sampler];

    textures.push_back(core::allocateShared<graphics::Texture>(imageView, sampler));
  }

  // Load meshes
  core::vector<core::shared_ptr<scene::Mesh>> meshes;
  meshes.reserve(tinyModel.meshes.size());
  
  for (const tinygltf::Mesh& tinyMesh : tinyModel.meshes)
  {
    core::shared_ptr<scene::Mesh> mesh = core::allocateShared<scene::Mesh>();

    for (const tinygltf::Primitive& tinyPrimitive : tinyMesh.primitives)
    {
      const core::uint32 verticesOffset = mesh->vertices.size();
      const core::uint32 primitiviesOffset = mesh->primitiveIndices.size();
      const core::uint32 vertexIndicestOffset = mesh->vertexIndices.size();
      const core::uint32 meshletsOffset = mesh->meshlets.size();

      core::vector<scene::Vertex> primtiveVertices = loadVerticesFromPrimitive(tinyModel, tinyPrimitive);
      core::vector<core::uint32> primitiveVertexIndices = loadVertexIndicesFromPrimitive(tinyModel, tinyPrimitive);

      buildMeshlets(primtiveVertices, primitiveVertexIndices, mesh->vertexIndices, mesh->primitiveIndices, mesh->meshlets);
      mesh->vertices.insert(mesh->vertices.end(), primtiveVertices.begin(), primtiveVertices.end());

      // !TODO
      core::shared_ptr<graphics::Material> submeshMaterial = core::allocateShared<graphics::Material>();
      if (tinyPrimitive.material != -1)
      {
        const tinygltf::Material& tinyMaterial = tinyModel.materials[tinyPrimitive.material];
        submeshMaterial->albedo = textures[tinyMaterial.pbrMetallicRoughness.baseColorTexture.index];
        if (tinyMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index == -1)
        {
          auto commandBuffer = core::allocateShared<graphics::Command_Buffer>(commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
          core::shared_ptr<graphics::Image> image = core::allocateShared<graphics::Image_2D>(
            commandBuffer,
            VK_FORMAT_R8G8_SNORM,
            graphics::Image::Mip_Data(VkExtent3D{ 1, 1, 1 }, core::array<core::byte, 2>{0, 100}),
            VK_SHARING_MODE_EXCLUSIVE);
          core::shared_ptr<graphics::Sampler> sampler = core::allocateShared<graphics::Sampler>(commandPool->getDevice(), graphics::csamlper_state::gMagMinMipLinearRepeat);

          submeshMaterial->metallicRoughness = core::allocateShared<graphics::Texture>(core::allocateShared<graphics::Image_View>(image, graphics::Image_Subresource_Range(image)), sampler);
        }
        else
        {
          submeshMaterial->metallicRoughness = textures[tinyMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index];
        }

        // !TODO
        if (tinyMaterial.normalTexture.index == -1)
        {
          auto commandBuffer = core::allocateShared<graphics::Command_Buffer>(commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
          core::shared_ptr<graphics::Image> image = core::allocateShared<graphics::Image_2D>(
            commandBuffer,
            VK_FORMAT_R8G8B8A8_SNORM,
            graphics::Image::Mip_Data(VkExtent3D{ 1, 1, 1 }, core::array<core::byte, 4>{0, 255, 0, 255}),
            VK_SHARING_MODE_EXCLUSIVE);
          core::shared_ptr<graphics::Sampler> sampler = core::allocateShared<graphics::Sampler>(commandPool->getDevice(), graphics::csamlper_state::gMagMinMipLinearRepeat);

          submeshMaterial->normal = core::allocateShared<graphics::Texture>(core::allocateShared<graphics::Image_View>(image, graphics::Image_Subresource_Range(image)), sampler);
        }
        else
        {
          submeshMaterial->normal = textures[tinyMaterial.normalTexture.index];
        }
      }

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

    meshes.push_back(mesh);
  }

  core::vector<core::shared_ptr<graphics::Mesh_Buffer>> meshBuffers;
  meshBuffers.reserve(tinyModel.meshes.size());
  for (core::shared_ptr<scene::Mesh> mesh : meshes)
  {
    auto commandBuffer = core::allocateShared<graphics::Command_Buffer>(commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    meshBuffers.push_back(core::allocateShared<graphics::Mesh_Buffer>(commandBuffer, *mesh));
  }

  // Load lights
  core::vector<scene::Point_Light_CPU> pointLights;
  for (const tinygltf::Light& tinyLight : tinyModel.lights)
  {
    if (tinyLight.type == "point")
    {
      pointLights.push_back(parsePointLight(tinyLight));
    }
  }

  // Load nodes
  for (const tinygltf::Node& tinyNode : tinyModel.nodes)
  {
    flecs::entity child = parseNode(tinyModel, node.world(), tinyNode, node, pointLights, meshes, meshBuffers);
    child.child_of(node);
  }
  
  if (!node.has<scene::Transform>())
    node.set<scene::Transform>(scene::Transform(node));
}

core::shared_ptr<graphics::Sampler> parseSampler(core::shared_ptr<const graphics::Device> device, const tinygltf::Sampler& tinySampler)
{
  return core::allocateShared<graphics::Sampler>(device, graphics::csamlper_state::gMagMinMipLinearRepeat);
}

core::shared_ptr<graphics::Image> parseImage(core::shared_ptr<graphics::Command_Pool> commandPool, const tinygltf::Image& tinyImage)
{
  auto commandBuffer = core::allocateShared<graphics::Command_Buffer>(commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);

  VkExtent3D extent;
  extent.width  = tinyImage.width;
  extent.height = tinyImage.height;
  extent.depth  = 1u;
  
  VkFormat format;
  switch (tinyImage.component)
  {
  case 1: format = VK_FORMAT_R8_SRGB; break;
  case 2: format = VK_FORMAT_R8G8_SRGB; break;
  case 3: format = VK_FORMAT_R8G8B8_SRGB; break;
  case 4: format = VK_FORMAT_R8G8B8A8_SRGB; break;
  default:
    core::logError(core::Debug::Channel::FileIO, "Failed to parse image from gltf");
    core::assert(false);
  }
  
  core::shared_ptr<graphics::Image> image = core::allocateShared<graphics::Image_2D>(
    commandBuffer,
    format,
    graphics::Image::Mip_Data(extent, core::span<const core::byte>(tinyImage.image)),
    scene::calculateMaximumMipLevelsCount(tinyImage.width, tinyImage.height),
    VK_SHARING_MODE_EXCLUSIVE);
  
  commandBuffer->begin();
  graphics::generateMipmaps(commandBuffer, image, VK_FILTER_LINEAR);
  commandBuffer->end();
  graphics::flush(commandBuffer);

  return image;
}

scene::Point_Light_CPU parsePointLight(const tinygltf::Light& tinyLight)
{
  scene::Point_Light_CPU pointLight;
  const DirectX::XMVECTOR color = tinyLight.color.size() > 0 ? DirectX::XMVectorSet(tinyLight.color[0], tinyLight.color[1], tinyLight.color[2], tinyLight.color[3]) : DirectX::XMVectorSplatOne();
  DirectX::XMStoreFloat3(&pointLight.intensity, DirectX::XMVectorScale(color, tinyLight.intensity));
  pointLight.sourceRadius = 0.5;
  return pointLight;
}

bool loadModelFromFile(tinygltf::Model& tinyModel, const char* path)
{
  tinygltf::TinyGLTF tinyLoader;
  std::string err;
  std::string warn;

  const bool loadResult = tinyLoader.LoadBinaryFromFile(&tinyModel, &err, &warn, path);

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
    core::logError(core::Debug::Channel::FileIO, "Failed to parse glTF");
    return false;
  }

  return true;
}


flecs::entity parseNode(tinygltf::Model&                                              tinyModel,
                        flecs::world                                                  world, 
                        const tinygltf::Node&                                         tinyNode,
                        flecs::entity_view                                            parent, 
                        const core::vector<scene::Point_Light_CPU>&                   pointLights,
                        const core::vector<core::shared_ptr<scene::Mesh>>&            meshes, 
                        const core::vector<core::shared_ptr<graphics::Mesh_Buffer>>&  meshBuffers)
{
  static core::uint32 uniqueNodeID = 0u;
  flecs::entity node(world, tinyNode.name.empty() ? ("Node " + std::to_string(uniqueNodeID++)).c_str() : tinyNode.name.c_str());
  
  if (tinyNode.light != -1)
  {
    node.set<scene::Point_Light_CPU>(pointLights[tinyNode.light]);
  }

  if (tinyNode.matrix.size() > 0)
  {
    scene::Transform transform(node);
    transform.setNodeToParent(DirectX::XMMATRIX(
      tinyNode.matrix[0], tinyNode.matrix[1], tinyNode.matrix[2], tinyNode.matrix[3], 
      tinyNode.matrix[4], tinyNode.matrix[5], tinyNode.matrix[6], tinyNode.matrix[7],
      tinyNode.matrix[8], tinyNode.matrix[9], tinyNode.matrix[10], tinyNode.matrix[11],
      tinyNode.matrix[12], tinyNode.matrix[13], tinyNode.matrix[14], tinyNode.matrix[15]));
    node.set<scene::Transform>(std::move(transform));
  }

  if (tinyNode.mesh != -1)
  {
    node.set<core::shared_ptr<scene::Mesh>>(meshes[tinyNode.mesh]);
    node.set<core::shared_ptr<graphics::Mesh_Buffer>>(meshBuffers[tinyNode.mesh]);
  }

  for (core::uint32 childIndex : tinyNode.children)
  {
    flecs::entity child = parseNode(tinyModel, world, tinyModel.nodes[childIndex], node, pointLights, meshes, meshBuffers);
    child.child_of(node);
  }
  return node;
}

core::vector<scene::Vertex> loadVerticesFromPrimitive(tinygltf::Model& tinyModel, const tinygltf::Primitive& tinyPrimitive)
{
  const core::uint32 primitiveVerticesCount = tinyModel.accessors[tinyPrimitive.attributes.begin()->second].count;
  core::vector<scene::Vertex> primitiveVertices(primitiveVerticesCount);

  for (const auto& attribute : tinyPrimitive.attributes)
  {
    const tinygltf::Accessor& tinyAccessor = tinyModel.accessors[attribute.second];
    if (attribute.first == "POSITION")
    {
      core::assert(tinyAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
      core::assert(tinyAccessor.type == TINYGLTF_TYPE_VEC3);
      loadBufferFromAccessor(tinyModel, tinyAccessor, reinterpret_cast<core::byte*>(&primitiveVertices[0].position.x), sizeof(DirectX::XMFLOAT3), sizeof(primitiveVertices[0]));
    }
    else if (attribute.first == "TEXCOORD_0")
    {
      core::assert(tinyAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
      core::assert(tinyAccessor.type == TINYGLTF_TYPE_VEC2);
      loadBufferFromAccessor(tinyModel, tinyAccessor, reinterpret_cast<core::byte*>(&primitiveVertices[0].texcoord.x), sizeof(DirectX::XMFLOAT2), sizeof(primitiveVertices[0]));
    }
    else if (attribute.first == "NORMAL")
    {
      core::assert(tinyAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
      core::assert(tinyAccessor.type == TINYGLTF_TYPE_VEC3);
      loadBufferFromAccessor(tinyModel, tinyAccessor, reinterpret_cast<core::byte*>(&primitiveVertices[0].normal.x), sizeof(DirectX::XMFLOAT3), sizeof(primitiveVertices[0]));
    }
  }

  return primitiveVertices;
}

core::vector<core::uint32> loadVertexIndicesFromPrimitive(tinygltf::Model& tinyModel, const tinygltf::Primitive& tinyPrimitive)
{
  const tinygltf::Accessor& tinyAccessor = tinyModel.accessors[tinyPrimitive.indices];
  const tinygltf::BufferView& tinyBufferView = tinyModel.bufferViews[tinyAccessor.bufferView];
  const tinygltf::Buffer& tinyBuffer = tinyModel.buffers[tinyBufferView.buffer];

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

void loadBufferFromAccessor(tinygltf::Model& tinyModel, const tinygltf::Accessor& tinyAccessor, core::byte* data, core::size_t elementSize, core::size_t byteStride)
{
  const tinygltf::BufferView& tinyBufferView = tinyModel.bufferViews[tinyAccessor.bufferView];
  const tinygltf::Buffer& tinyBuffer = tinyModel.buffers[tinyBufferView.buffer];

  const core::byte* bufferData = reinterpret_cast<const core::byte*>(&tinyBuffer.data[tinyAccessor.byteOffset + tinyBufferView.byteOffset]);
  for (core::size_t i = 0u; i < tinyAccessor.count; ++i)
  {
    std::memcpy(data, bufferData, elementSize);
    data += byteStride;
    bufferData += tinyBufferView.byteStride;
  }
}

void buildMeshlets(const core::vector<scene::Vertex>&  submeshVertices,
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