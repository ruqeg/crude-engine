#define TINYGLTF_NOEXCEPTION
#include <tiny_gltf.h>
#include <vulkan/vulkan.h>
#include <algorithm>
#include <functional>
#include <meshoptimizer.h>
#include <limits>
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

  // Load samples
  std::vector<core::shared_ptr<graphics::Sampler>> samplers(m_tinyModel.meshes.size());
  for (core::size_t samplerIndex = 0u; samplerIndex < m_tinyModel.samplers.size(); ++samplerIndex)
  {
    samplers[samplerIndex] = parseSampler(m_tinyModel.samplers[samplerIndex]);
  }

  // Load images
  std::vector<core::shared_ptr<graphics::Image>> images(m_tinyModel.images.size());
  for (core::size_t imageIndex = 0u; imageIndex < m_tinyModel.images.size(); ++imageIndex)
  {
    images[imageIndex] = parseImage(m_tinyModel.images[imageIndex]);
  }

  // Load textures

  // Load meshes
  std::vector<scene::Mesh_Buffer> meshBuffers(m_tinyModel.meshes.size());
  std::vector<scene::Mesh> meshes(m_tinyModel.meshes.size());

  for (core::size_t meshIndex = 0u; meshIndex < meshes.size(); ++meshIndex)
  {
    const tinygltf::Mesh& tinyMesh = m_tinyModel.meshes[meshIndex];
    scene::Mesh& mesh = meshes[meshIndex];

    core::uint32 verticesOffset = 0u;
    core::uint32 primitiviesOffset = 0u;
    core::uint32 vertexIndicestOffset = 0u;
    core::uint32 meshletsOffset = 0u;

    for (const tinygltf::Primitive& tinyPrimitive : tinyMesh.primitives)
    {
      core::vector<scene::Vertex> primtiveVertices = loadVerticesFromPrimitive(tinyPrimitive);
      core::vector<core::uint32> primitiveVertexIndices = loadVertexIndicesFromPrimitive(tinyPrimitive);

      verticesOffset += mesh.vertices.size();
      primitiviesOffset += mesh.primitiveIndices.size();
      vertexIndicestOffset += mesh.vertexIndices.size();
      meshletsOffset += mesh.meshlets.size();

      buildMeshlets(primtiveVertices, primitiveVertexIndices, mesh.vertexIndices, mesh.primitiveIndices, mesh.meshlets);
      mesh.vertices.insert(mesh.vertices.end(), primtiveVertices.begin(), primtiveVertices.end());

      mesh.subMeshes.push_back(scene::Sub_Mesh{
        .vertexOffset = verticesOffset,
        .vertexCount = static_cast<core::uint32>(primtiveVertices.size()),
        .lodCount = 1u,
        .lods = core::array<scene::Sub_Mesh_Lod, 8> { scene::Sub_Mesh_Lod {
          .indexOffset = vertexIndicestOffset,
          .indexCount = static_cast<core::uint32>(mesh.vertexIndices.size() - vertexIndicestOffset),
          .meshletOffset = meshletsOffset,
          .meshletCount = static_cast<core::uint32>(mesh.meshlets.size() - meshletsOffset),
        } }
        });
    }
  }

  //for (core::size_t meshIndex = 0u; meshIndex < meshes.size(); ++meshIndex)
  //{
  //  scene::Mesh_Buffer& meshBuffer = meshBuffers[meshIndex];
  //  meshBuffers[meshIndex].meshletsBuffer;
  //  meshBuffers[meshIndex].primitiveIndicesBuffer;
  //  meshBuffers[meshIndex].vertexIndicesBuffer;
  //  meshBuffers[meshIndex].verticesBuffer;
  //}


  // Load nodes

  core::shared_ptr<scene::Scene> scene = core::allocateShared<scene::Scene>();
  return scene;
}

core::shared_ptr<graphics::Sampler> GLTF_Loader::parseSampler(const tinygltf::Sampler& tinySampler)
{
  return core::allocateShared<graphics::Sampler>(graphics::csamlper_state::gMagMinMipLinearRepeat);
}

core::shared_ptr<graphics::Image> GLTF_Loader::parseImage(const tinygltf::Image& tinyImage)
{
  core::shared_ptr<core::byte[]> imageByte = core::allocateShared<core::byte[]>(tinyImage.image.size());

  auto commandBuffer = core::allocateShared<graphics::Command_Buffer>(m_commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  
  core::shared_ptr<scene::Image> sceneImage = core::allocateShared<scene::Image>(
    imageByte, scene::Image_Format::IMAGE_FORMAT_RGB_ALPHA, tinyImage.width, tinyImage.height);

  VkExtent3D extent;
  extent.width  = tinyImage.width;
  extent.height = tinyImage.height;
  extent.depth  = 1u;
  
  core::shared_ptr<graphics::Image> image = core::allocateShared<graphics::Image_2D>(
    commandBuffer,
    VK_FORMAT_R8G8B8A8_SRGB,
    graphics::Image::Mip_Data(extent, sceneImage->getTexelsSpan()),
    sceneImage->calculateMaximumMipLevelsCount(),
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

core::shared_ptr<scene::Node> GLTF_Loader::loadNodesFromModel(const tinygltf::Node& tinyNode, DirectX::FXMMATRIX parentToParent)
{
  core::shared_ptr<scene::Node> node = core::allocateShared<scene::Node>();

  const DirectX::XMMATRIX nodeToParent = calculateNodeToParent(tinyNode, parentToParent);
  DirectX::XMStoreFloat4x4(&node->m_nodeToParent, nodeToParent);

  node->m_geometryIndex = tinyNode.mesh;

  for (core::uint32 childIndex : tinyNode.children)
  {
    node->m_nodes.push_back(loadNodesFromModel(m_tinyModel.nodes[childIndex], nodeToParent));
  }
  return node;
}

core::vector<core::shared_ptr<scene::Texture>> GLTF_Loader::loadTexturesFromModel()
{
  core::vector<core::shared_ptr<scene::Texture>> textures;
  textures.reserve(m_tinyModel.textures.size());
  for (const tinygltf::Texture& tinyTexture : m_tinyModel.textures)
  {
    const tinygltf::Image& tinyImage = m_tinyModel.images[tinyTexture.source];
    core::shared_ptr<core::byte[]> imageByte = core::allocateShared<core::byte[]>(tinyImage.image.size());
    std::memcpy(imageByte.get(), tinyImage.image.data(), static_cast<core::size_t>(tinyImage.image.size()));

    core::shared_ptr<scene::Image> image = core::allocateShared<scene::Image>(imageByte, scene::Image_Format::IMAGE_FORMAT_RGB_ALPHA, tinyImage.width, tinyImage.height);
    core::shared_ptr<graphics::Sampler_State> samplerState = core::allocateShared<graphics::Sampler_State>(graphics::csamlper_state::gMagMinMipLinearRepeat);

    core::shared_ptr<scene::Texture> texture = core::allocateShared<scene::Texture>();
    texture->setImage(image);
    texture->setSamplerState(samplerState);
    textures.push_back(texture);
  }
  return textures;
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

DirectX::XMMATRIX GLTF_Loader::calculateNodeToParent(const tinygltf::Node& tinyNode, DirectX::FXMMATRIX nodeToParent)
{
  if (tinyNode.matrix.size() == 0)
    return nodeToParent;

  return DirectX::XMMatrixTranspose(DirectX::XMMatrixSet(
    tinyNode.matrix[0], tinyNode.matrix[1], tinyNode.matrix[2], tinyNode.matrix[3],
    tinyNode.matrix[4], tinyNode.matrix[5], tinyNode.matrix[6], tinyNode.matrix[7],
    tinyNode.matrix[8], tinyNode.matrix[9], tinyNode.matrix[10], tinyNode.matrix[11],
    tinyNode.matrix[12], tinyNode.matrix[13], tinyNode.matrix[14], tinyNode.matrix[15]));
}

}