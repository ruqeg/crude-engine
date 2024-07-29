#define TINYGLTF_NOEXCEPTION
#include <tiny_gltf.h>
#include <vulkan/vulkan.h>
#include <algorithm>
#include <functional>
#include <meshoptimizer.h>
#include <limits>
#include <flecs.h>
#include <directxmath/DirectXMath.h>
#undef assert

module crude.resources.gltf_loader;

import crude.graphics.device;
import crude.graphics.sampler;
import crude.graphics.command_buffer;
import crude.graphics.generate_mipmaps;
import crude.graphics.command_pool;
import crude.graphics.flush;
import crude.graphics.image_2d;
import crude.graphics.image_view;
import crude.graphics.texture;
import crude.graphics.mesh_buffer;

namespace crude::resources
{

GLTF_Loader::GLTF_Loader(core::shared_ptr<graphics::Command_Pool> commandPool)
  : m_device(commandPool->getDevice())
  , m_commandPool(commandPool)
{}

core::Optional<core::shared_ptr<scene::Scene>> GLTF_Loader::loadSceneFromFile(const char* path)
{
  if (!loadModelFromFile(path))
    return core::nullopt;

  core::shared_ptr<scene::Scene> scene = core::allocateShared<scene::Scene>();

  // Load samples
  core::vector<core::shared_ptr<graphics::Sampler>> samplers;
  samplers.reserve(m_tinyModel.samplers.size());
  for (const tinygltf::Sampler& tinySampler : m_tinyModel.samplers)
  {
    samplers.push_back(parseSampler(tinySampler));
  }

  // Load images
  core::vector<core::shared_ptr<graphics::Image>> images;
  images.reserve(m_tinyModel.images.size());
  for (const tinygltf::Image& tinyImage : m_tinyModel.images)
  {
    images.push_back(parseImage(tinyImage));
  }

  // Load textures
  core::vector<core::shared_ptr<graphics::Texture>> textures;
  textures.reserve(m_tinyModel.textures.size());
  for (const tinygltf::Texture& tinyTexture : m_tinyModel.textures)
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
  meshes.reserve(m_tinyModel.meshes.size());

  for (const tinygltf::Mesh& tinyMesh : m_tinyModel.meshes)
  {
    core::shared_ptr<scene::Mesh> mesh = core::allocateShared<scene::Mesh>();

    core::uint32 verticesOffset = 0u;
    core::uint32 primitiviesOffset = 0u;
    core::uint32 vertexIndicestOffset = 0u;
    core::uint32 meshletsOffset = 0u;

    for (const tinygltf::Primitive& tinyPrimitive : tinyMesh.primitives)
    {
      core::vector<scene::Vertex> primtiveVertices = loadVerticesFromPrimitive(tinyPrimitive);
      core::vector<core::uint32> primitiveVertexIndices = loadVertexIndicesFromPrimitive(tinyPrimitive);

      verticesOffset += mesh->vertices.size();
      primitiviesOffset += mesh->primitiveIndices.size();
      vertexIndicestOffset += mesh->vertexIndices.size();
      meshletsOffset += mesh->meshlets.size();

      buildMeshlets(primtiveVertices, primitiveVertexIndices, mesh->vertexIndices, mesh->primitiveIndices, mesh->meshlets);
      mesh->vertices.insert(mesh->vertices.end(), primtiveVertices.begin(), primtiveVertices.end());

      core::shared_ptr<graphics::Texture> submeshTexture;
      if (tinyPrimitive.material != -1)
      {
        submeshTexture = textures[m_tinyModel.materials[tinyPrimitive.material].pbrMetallicRoughness.baseColorTexture.index];
      }

      mesh->subMeshes.push_back(scene::Sub_Mesh{
        .vertexOffset = verticesOffset,
        .vertexCount = static_cast<core::uint32>(primtiveVertices.size()),
        .lodCount = 1u,
        .lods = core::array<scene::Sub_Mesh_Lod, 8> { scene::Sub_Mesh_Lod {
          .indexOffset = vertexIndicestOffset,
          .indexCount = static_cast<core::uint32>(mesh->vertexIndices.size() - vertexIndicestOffset),
          .meshletOffset = meshletsOffset,
          .meshletCount = static_cast<core::uint32>(mesh->meshlets.size() - meshletsOffset),
        } },
        .texture = submeshTexture
        });
    }

    meshes.push_back(mesh);
  }

  core::vector<core::shared_ptr<graphics::Mesh_Buffer>> meshBuffers;
  meshBuffers.reserve(m_tinyModel.meshes.size());
  for (core::shared_ptr<scene::Mesh> mesh : meshes)
  {
    auto commandBuffer = core::allocateShared<graphics::Command_Buffer>(m_commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    meshBuffers.push_back(core::allocateShared<graphics::Mesh_Buffer>(commandBuffer, *mesh));
  }

  // Load nodes
  core::vector<core::shared_ptr<scene::Node>> nodes;
  nodes.reserve(m_tinyModel.nodes.size());
  for (const tinygltf::Node& tinyNode : m_tinyModel.nodes)
  {
    nodes.push_back(parseNode(tinyNode, scene, meshes, meshBuffers));
  }

  scene->setNodes(nodes);
  return scene;
}

core::shared_ptr<graphics::Sampler> GLTF_Loader::parseSampler(const tinygltf::Sampler& tinySampler)
{
  return core::allocateShared<graphics::Sampler>(m_device, graphics::csamlper_state::gMagMinMipLinearRepeat);
}

core::shared_ptr<graphics::Image> GLTF_Loader::parseImage(const tinygltf::Image& tinyImage)
{
  auto commandBuffer = core::allocateShared<graphics::Command_Buffer>(m_commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);

  VkExtent3D extent;
  extent.width  = tinyImage.width;
  extent.height = tinyImage.height;
  extent.depth  = 1u;
  
  core::shared_ptr<graphics::Image> image = core::allocateShared<graphics::Image_2D>(
    commandBuffer,
    VK_FORMAT_R8G8B8A8_SRGB,
    graphics::Image::Mip_Data(extent, core::span<const core::byte>(tinyImage.image)),
    scene::calculateMaximumMipLevelsCount(tinyImage.width, tinyImage.height),
    VK_SHARING_MODE_EXCLUSIVE);
  
  commandBuffer->begin();
  graphics::generateMipmaps(commandBuffer, image, VK_FILTER_LINEAR);
  commandBuffer->end();
  flush(commandBuffer);

  return image;
}

bool GLTF_Loader::loadModelFromFile(const char* path)
{
  tinygltf::TinyGLTF tinyLoader;
  std::string err;
  std::string warn;

  const bool loadResult = tinyLoader.LoadBinaryFromFile(&m_tinyModel, &err, &warn, path);

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

core::shared_ptr<scene::Node> GLTF_Loader::parseNode(const tinygltf::Node& tinyNode, core::shared_ptr<scene::Scene> scene, const core::vector<core::shared_ptr<scene::Mesh>>& meshes, const core::vector<core::shared_ptr<graphics::Mesh_Buffer>>& meshBuffers)
{
  core::shared_ptr<scene::Node> node = core::allocateShared<scene::Node>(scene, tinyNode.name.c_str());

  if (tinyNode.mesh != -1)
  {
    node->getEntity().set<core::shared_ptr<scene::Mesh>>(meshes[tinyNode.mesh]);
    node->getEntity().set<core::shared_ptr<graphics::Mesh_Buffer>>(meshBuffers[tinyNode.mesh]);
  }

  for (core::uint32 childIndex : tinyNode.children)
  {
    core::shared_ptr<scene::Node> child = parseNode(m_tinyModel.nodes[childIndex], scene, meshes, meshBuffers);
    child->setParent(child);
    node->addChild(child);
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

void GLTF_Loader::buildMeshlets(const core::vector<scene::Vertex>& vertices,
  const core::vector<core::uint32>& vertexIndices,
  core::vector<core::uint32>& meshVertexIndices,
  core::vector<core::uint8>& meshPrimitiveIndices,
  core::vector<scene::Meshlet>& meshMeshlets)
{
  constexpr core::size_t maxVertices = 64u;
  constexpr core::size_t maxTriangles = 124u;
  constexpr core::float32 coneWeight = 0.0f;

  meshopt_setAllocator(core::defaultAllocate, core::defaultDeallocate);

  const core::uint32 meshVertexIndicesOffset = meshVertexIndices.size();
  const core::uint32 meshPrimitiveIndicesOffset = meshPrimitiveIndices.size();
  const core::uint32 meshMeshletOffset = meshMeshlets.size();

  const core::size_t maxMeshlets = meshopt_buildMeshletsBound(vertexIndices.size(), maxVertices, maxTriangles);

  core::vector<meshopt_Meshlet> zeuxMeshlets(maxMeshlets);
  meshVertexIndices.resize(meshVertexIndicesOffset + maxMeshlets * maxVertices);
  meshPrimitiveIndices.resize(meshPrimitiveIndicesOffset + maxMeshlets * maxTriangles * 3);

  const core::size_t meshletCount = meshopt_buildMeshlets(zeuxMeshlets.data(), meshVertexIndices.data(), meshPrimitiveIndices.data(), vertexIndices.data(),
    vertexIndices.size(), &vertices[0].position.x, vertices.size(), sizeof(scene::Vertex), maxVertices, maxTriangles, coneWeight);

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
      .vertexOffset = zeuxMeshlet.vertex_offset,
      .primitiveOffest = zeuxMeshlet.triangle_offset,
      });
  }
}

}