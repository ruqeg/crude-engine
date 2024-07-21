module;

#define TINYGLTF_NOEXCEPTION
#include <tiny_gltf.h>
#include <algorithm>
#include <functional>
#include <meshoptimizer.h>
#include <limits>
#include <directxmath/DirectXMath.h>
#undef assert

export module crude.resources.world_loader;

export import crude.scene.world;
export import crude.core.optional;
import crude.core.logger;
import crude.core.assert;

namespace crude::resources
{

DirectX::XMFLOAT3 loadTinyTranslation(const tinygltf::Node& node)
{
  if (node.translation.size() == 0u)
    return DirectX::XMFLOAT3();
  return DirectX::XMFLOAT3(node.translation[0], node.translation[1], node.translation[2]);
}

DirectX::XMFLOAT3 loadTinyRotation(const tinygltf::Node& node)
{
  if (node.rotation.size() == 0u)
    return DirectX::XMFLOAT3();
  return DirectX::XMFLOAT3(node.rotation[0], node.rotation[1], node.rotation[2]);
}

bool loadBinaryFromFile(tinygltf::Model& tinyModel, const char* path)
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

DirectX::XMMATRIX calculateNodeToParent(const tinygltf::Node& tinyNode, DirectX::FXMMATRIX nodeToParent)
{
  if (tinyNode.matrix.size() == 0)
    return nodeToParent;
  
  return DirectX::XMMatrixSet(
    tinyNode.matrix[0], tinyNode.matrix[1], tinyNode.matrix[2], tinyNode.matrix[3],
    tinyNode.matrix[4], tinyNode.matrix[5], tinyNode.matrix[6], tinyNode.matrix[7],
    tinyNode.matrix[8], tinyNode.matrix[9], tinyNode.matrix[10], tinyNode.matrix[11],
    tinyNode.matrix[12], tinyNode.matrix[13], tinyNode.matrix[14], tinyNode.matrix[15]);
}

bool loadCameraFromNode(const tinygltf::Model& tinyModel, const tinygltf::Node& tinyNode, scene::Camera& camera)
{
  if (tinyNode.camera == -1)
    return false;

  const tinygltf::Camera& tinyCamera = tinyModel.cameras[tinyNode.camera];

  camera.setPosition(loadTinyTranslation(tinyNode));
  camera.setRotation(loadTinyRotation(tinyNode));
  if (tinyCamera.type == "perspective")
  {
    camera.calculateViewToClipMatrix(tinyCamera.perspective.yfov, tinyCamera.perspective.aspectRatio, tinyCamera.perspective.znear, tinyCamera.perspective.zfar);
  }
  return true;
}

void loadhAttributeFromMesh(const tinygltf::Model&     tinyModel,
                           const tinygltf::Primitive&  tinyPrimitive, 
                           const char*                 attributeName, 
                           core::byte*                 data, 
                           core::uint32                byteSize, 
                           core::uint32                byteStride)
{
  const auto tinyAttributeIt = tinyPrimitive.attributes.find(attributeName);
  core::assert(tinyAttributeIt != tinyPrimitive.attributes.end());

  const core::uint32 tinyAttribute = tinyAttributeIt->second;
  const tinygltf::Accessor& tinyAccessor = tinyModel.accessors[tinyAttribute];
  const tinygltf::BufferView& tinyBufferView = tinyModel.bufferViews[tinyAccessor.bufferView];
  const tinygltf::Buffer& tinyBuffer = tinyModel.buffers[tinyBufferView.buffer];

  const core::byte* bufferData = reinterpret_cast<const core::byte*>(&tinyBuffer.data[tinyAccessor.byteOffset + tinyBufferView.byteOffset]);
  for (core::size_t i = 0u; i < tinyAccessor.count; ++i)
  {
    std::memcpy(data, bufferData, byteSize);
    data += byteStride;
    bufferData += tinyBufferView.byteStride;
  }
}
void loadIndicesFromMesh(const tinygltf::Model&       tinyModel,
                         const tinygltf::Primitive&   tinyPrimitive, 
                         core::vector<core::uint32>&  indices)
{
  const tinygltf::Accessor& tinyAccessor = tinyModel.accessors[tinyPrimitive.indices];
  const tinygltf::BufferView& tinyBufferView = tinyModel.bufferViews[tinyAccessor.bufferView];
  const tinygltf::Buffer& tinyBuffer = tinyModel.buffers[tinyBufferView.buffer];

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
}

void calculateMeshletsFromMesh(core::shared_ptr<scene::Geometry> geometry, core::uint32 meshVertexOffset, core::vector<core::uint32> meshIndices)
{
  constexpr core::size_t maxVertices = 64u;
  constexpr core::size_t maxTriangles = 124u;
  constexpr core::float32 coneWeight = 0.0f;

  meshopt_setAllocator(core::defaultAllocate, core::defaultDeallocate);

  const core::size_t maxMeshlets = meshopt_buildMeshletsBound(meshIndices.size(), maxVertices, maxTriangles);
  core::vector<meshopt_Meshlet> zeuxMeshlets(maxMeshlets);
  const core::uint32 meshVertexIndicesOffset = geometry->m_vertexIndices.size();
  const core::uint32 meshPrimitiveIndicesOffset = geometry->m_primitiveIndices.size();
  geometry->m_vertexIndices.resize(meshVertexIndicesOffset + maxMeshlets * maxVertices);
  geometry->m_primitiveIndices.resize(meshPrimitiveIndicesOffset + maxMeshlets * maxTriangles * 3);

  const core::size_t meshletCount = meshopt_buildMeshlets(zeuxMeshlets.data(), geometry->m_vertexIndices.data(), geometry->m_primitiveIndices.data(), meshIndices.data(),
    meshIndices.size(), &geometry->m_vertices[meshVertexOffset].position.x, geometry->m_vertices.size(), sizeof(scene::Vertex), maxVertices, maxTriangles, coneWeight);

  const meshopt_Meshlet& zeuxLastMeshlet = zeuxMeshlets[meshletCount - 1];
  geometry->m_vertexIndices.resize(meshVertexIndicesOffset + zeuxLastMeshlet.vertex_offset + zeuxLastMeshlet.vertex_count);
  geometry->m_primitiveIndices.resize(meshPrimitiveIndicesOffset + zeuxLastMeshlet.triangle_offset + ((zeuxLastMeshlet.triangle_count * 3 + 3) & ~3));
  zeuxMeshlets.resize(meshletCount);

  const core::uint32 meshMeshletOffset = geometry->m_meshlets.size();
  geometry->m_meshlets.reserve(meshMeshletOffset + meshletCount);

  for (const meshopt_Meshlet& zeuxMeshlet : zeuxMeshlets)
  {
    core::assert(zeuxMeshlet.vertex_count <= std::numeric_limits<core::uint8>::max());
    core::assert(zeuxMeshlet.triangle_count <= std::numeric_limits<core::uint8>::max());

    meshopt_optimizeMeshlet(
      &geometry->m_vertexIndices[meshVertexIndicesOffset + zeuxMeshlet.vertex_offset],
      &geometry->m_primitiveIndices[meshPrimitiveIndicesOffset + zeuxMeshlet.triangle_offset],
      zeuxMeshlet.triangle_count,
      zeuxMeshlet.vertex_count);

    geometry->m_meshlets.push_back(scene::Meshlet{
      .vertexCount = static_cast<core::uint8>(zeuxMeshlet.vertex_count),
      .primitiveCount = static_cast<core::uint8>(zeuxMeshlet.triangle_count),
      .vertexOffset = zeuxMeshlet.vertex_offset,
      .primitiveOffest = zeuxMeshlet.triangle_offset,
      });
  }
}

bool loadMaterialFromMesh(const tinygltf::Model&      tinyModel,
                          const tinygltf::Primitive&  tinyPrimitive,
                          scene::Texture&             texture)
{
  if (tinyPrimitive.material != -1)
    return false;

  const tinygltf::Material& tinyMaterial = tinyModel.materials[tinyPrimitive.material];
  const tinygltf::Texture& tinyTexture = tinyModel.textures[tinyMaterial.pbrMetallicRoughness.baseColorTexture.index];
  const tinygltf::Image& tinyImage = tinyModel.images[tinyTexture.source];
  core::shared_ptr<core::byte[]> imageByte = core::allocateShared<core::byte[]>(tinyImage.image.size());
  std::memcpy(imageByte.get(), tinyImage.image.data(), static_cast<core::size_t>(tinyImage.image.size()));

  core::shared_ptr<scene::Image> image = core::allocateShared<scene::Image>(imageByte, scene::Image_Format::IMAGE_FORMAT_RGB_ALPHA, tinyImage.width, tinyImage.height);
  core::shared_ptr<graphics::Sampler_State> samplerState = core::allocateShared<graphics::Sampler_State>(graphics::csamlper_state::gMagMinMipLinearRepeat);
  texture.setImage(image);
  texture.setSamplerState(samplerState);
  return true;
}

bool loadMeshFromNode(const tinygltf::Model& tinyModel, const tinygltf::Node& tinyNode, core::shared_ptr<scene::Node> node)
{
  if (tinyNode.mesh == -1)
    return false;

  const tinygltf::Mesh& tinyMesh = tinyModel.meshes[tinyNode.mesh];
  const tinygltf::Primitive tinyPrimitive = tinyMesh.primitives.front();

  const core::uint32 vertexIndicestOffset = node->m_geometry->m_vertexIndices.size();
  const core::uint32 meshletOffset        = node->m_geometry->m_meshlets.size();
  const core::uint32 meshVerticesSize     = tinyModel.accessors[tinyPrimitive.attributes.begin()->second].count;
  const core::uint32 vertexOffset         = node->m_geometry->m_vertices.size();
  node->m_geometry->m_vertices.resize(vertexOffset + meshVerticesSize);

  core::vector<core::uint32> meshIndices;
  meshIndices.resize(tinyModel.accessors[tinyPrimitive.indices].count);

  loadhAttributeFromMesh(tinyModel, tinyPrimitive, "POSITION", reinterpret_cast<core::byte*>(&node->m_geometry->m_vertices[vertexOffset].position), sizeof(DirectX::XMFLOAT3), sizeof(scene::Vertex));
  loadhAttributeFromMesh(tinyModel, tinyPrimitive, "TEXCOORD_0", reinterpret_cast<core::byte*>(&node->m_geometry->m_vertices[vertexOffset].texcoord), sizeof(DirectX::XMFLOAT2), sizeof(scene::Vertex));
  loadIndicesFromMesh(tinyModel, tinyPrimitive, meshIndices);
  calculateMeshletsFromMesh(node->m_geometry, vertexOffset, meshIndices);

  scene::Texture texture;
  if (loadMaterialFromMesh(tinyModel, tinyPrimitive, texture))
  {
    node->m_texturePerMeshes.push_back(core::allocateShared<scene::Texture>(texture));
  }
  else
  {
    node->m_texturePerMeshes.push_back(nullptr);
  }

  node->m_geometry->m_meshes.push_back(scene::Mesh{
    .vertexOffset = vertexOffset,
    .vertexCount = static_cast<core::uint32>(node->m_geometry->m_vertices.size()),
    .lodCount = 1u,
    .lods = core::array<scene::Mesh_Lod, 8> { scene::Mesh_Lod {
      .indexOffset = vertexIndicestOffset,
      .indexCount = static_cast<core::uint32>(node->m_geometry->m_vertexIndices.size() - vertexIndicestOffset),
      .meshletOffset = meshletOffset,
      .meshletCount = static_cast<core::uint32>(node->m_geometry->m_meshlets.size() - meshletOffset),
    } }
    });
}

void loadSceneFromModel(const tinygltf::Model&                         tinyModel, 
                        const tinygltf::Node&                          tinyNode, 
                        core::vector<core::shared_ptr<scene::Camera>>  cameras,
                        core::shared_ptr<scene::Node>                  node,
                        DirectX::FXMMATRIX                             parentToModel)
{
  const DirectX::XMMATRIX nodeToParent = calculateNodeToParent(tinyNode, parentToModel);
  DirectX::XMFLOAT4X4 nodeToParentFloat4X4;
  DirectX::XMStoreFloat4x4(&nodeToParentFloat4X4, nodeToParent);
  node->m_meshesToModel.push_back(nodeToParentFloat4X4);

  scene::Camera camera;
  if (loadCameraFromNode(tinyModel, tinyNode, camera))
    cameras.push_back(core::allocateShared<scene::Camera>(camera));

  loadMeshFromNode(tinyModel, tinyNode, node);

  for (core::uint32 childNode : tinyNode.children)
  {
    loadSceneFromModel(tinyModel, tinyModel.nodes[childNode], cameras, node, nodeToParent);
  }
}
}

export namespace crude::resources
{

core::Optional<core::shared_ptr<scene::World>> loadWorld(const char* path) noexcept
{
  tinygltf::Model tinyModel;
  if (!loadBinaryFromFile(tinyModel, path))
    return core::nullopt;

  core::shared_ptr<scene::Node> node = core::allocateShared<scene::Node>();
  node->m_geometry = core::allocateShared<scene::Geometry>();
  core::vector<core::shared_ptr<scene::Camera>> cameras;

  tinygltf::Scene& tinyScene = tinyModel.scenes[tinyModel.defaultScene];
  for (core::uint32 tinyNodeIndex : tinyScene.nodes)
  {
    const tinygltf::Node& tinyNode = tinyModel.nodes[tinyNodeIndex];
    const DirectX::XMMATRIX nodeToParent = calculateNodeToParent(tinyNode, DirectX::XMMatrixIdentity());
    loadSceneFromModel(tinyModel, tinyNode, cameras, node, nodeToParent);
  }

  core::shared_ptr<scene::World> world = core::allocateShared<scene::World>();
  world->setNode(node);
  if (!cameras.empty())
    world->setCamera(cameras.front());
  return world;
}

}