module;

#define TINYGLTF_NOEXCEPTION
#include <tiny_gltf.h>
#include <algorithm>
#include <functional>
#include <meshoptimizer.h>
#include <limits>
#undef assert

export module crude.resources.world_loader;

export import crude.scene.world;
export import crude.core.optional;
import crude.core.logger;
import crude.core.assert;

namespace crude::resources
{

math::Vector loadTinyTranslation(const tinygltf::Node& node)
{
  if (node.translation.size() == 0u)
    return math::svector::zero();
  return math::svector::set(node.translation[0], node.translation[1], node.translation[2], 0.f);
}

math::Vector loadTinyRotation(const tinygltf::Node& node)
{
  if (node.rotation.size() == 0u)
    return math::svector::zero();
  return math::svector::set(node.rotation[0], node.rotation[1], node.rotation[2], 0.f);
}

}

export namespace crude::resources
{

// !TODO

core::Optional<core::shared_ptr<scene::World>> loadWorld(const char* path) noexcept
{
  auto loadBinaryFromFile = [path](tinygltf::Model& model) -> bool {
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    const bool loadResult = loader.LoadBinaryFromFile(&model, &err, &warn, path);

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
    };

  tinygltf::Model tinyModel;
  if (!loadBinaryFromFile(tinyModel))
    return core::nullopt;

  auto calculateNodeToParent = [](const tinygltf::Node& tinyNode, math::Matrix nodeToParent) -> math::Matrix {
    if (tinyNode.matrix.size() == 0)
      return nodeToParent;

    return math::Matrix(
      tinyNode.matrix[0], tinyNode.matrix[1], tinyNode.matrix[2], tinyNode.matrix[3],
      tinyNode.matrix[4], tinyNode.matrix[5], tinyNode.matrix[6], tinyNode.matrix[7],
      tinyNode.matrix[8], tinyNode.matrix[9], tinyNode.matrix[10], tinyNode.matrix[11],
      tinyNode.matrix[12], tinyNode.matrix[13], tinyNode.matrix[14], tinyNode.matrix[15]);
    };

  core::shared_ptr<scene::World> world = core::allocateShared<scene::World>();
  core::shared_ptr<scene::Geometry> geometry = core::allocateShared<scene::Geometry>();
  std::vector<core::shared_ptr<scene::Camera>> cameras;

  std::function<void(tinygltf::Node&, math::Matrix)> loadScene;
  loadScene = [&](tinygltf::Node& tinyNode, math::Matrix parentToModel) {
    auto loadCamera = [&]() -> core::shared_ptr<scene::Camera> {
      if (tinyNode.camera == -1)
        return nullptr;

      const tinygltf::Camera& tinyCamera = tinyModel.cameras[tinyNode.camera];

      // !TODO
      core::shared_ptr<scene::Camera> camera = core::allocateShared<scene::Camera>();
      camera->setName(reinterpret_cast<const char8_t*>(tinyCamera.name.c_str()));
      camera->setPosition(loadTinyTranslation(tinyNode));
      camera->setRotation(loadTinyRotation(tinyNode));
      if (tinyCamera.type == "perspective")
      {
        camera->calculateViewToClipMatrix(tinyCamera.perspective.yfov, tinyCamera.perspective.aspectRatio, tinyCamera.perspective.znear, tinyCamera.perspective.zfar);
      }
      return camera;
      };

    auto loadMesh = [&](core::shared_ptr<scene::Texture> texture, core::shared_ptr<scene::Geometry> geometry) -> void {
      if (tinyNode.mesh == -1)
        return;

      const tinygltf::Mesh& tinyMesh = tinyModel.meshes[tinyNode.mesh];
      const tinygltf::Primitive tinyPrimitive = tinyMesh.primitives.front();

      auto loadMeshAttributeData = [&](const char* attributeName, core::byte* data, core::uint32 byteSize, core::uint32 byteStride) -> void {
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
        };
      auto loadMeshIndicesData = [&](core::vector<core::uint32>& indices) -> void {
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
        };
      auto loadMeshlets = [&tinyMesh, &tinyModel](core::shared_ptr<scene::Geometry> geometry, core::uint32 meshVertexOffset, core::vector<core::uint32> meshIndices) -> void {
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
        };

      auto loadMaterial = [&tinyModel, &tinyPrimitive]() -> core::shared_ptr<scene::Texture> {
        const tinygltf::Material& tinyMaterial = tinyModel.materials[tinyPrimitive.material];
        const tinygltf::Texture& tinyTexture = tinyModel.textures[tinyMaterial.pbrMetallicRoughness.baseColorTexture.index];
        const tinygltf::Image& tinyImage = tinyModel.images[tinyTexture.source];
        core::shared_ptr<core::byte[]> imageByte = core::allocateShared<core::byte[]>(tinyImage.image.size());
        std::memcpy(imageByte.get(), tinyImage.image.data(), static_cast<core::size_t>(tinyImage.image.size()));

        core::shared_ptr<scene::Image> mimage = core::allocateShared<scene::Image>(imageByte, scene::Image_Format::IMAGE_FORMAT_RGB_ALPHA, tinyImage.width, tinyImage.height);
        core::shared_ptr<graphics::Sampler_State> samplerState = core::allocateShared<graphics::Sampler_State>(graphics::csamlper_state::gMagMinMipLinearRepeat);
        return core::allocateShared<scene::Texture>(mimage, samplerState);
        };

      const core::uint32 vertexIndicestOffset = geometry->m_vertexIndices.size();
      const core::uint32 meshletOffset = geometry->m_meshlets.size();
      const core::uint32 meshVerticesSize = tinyModel.accessors[tinyPrimitive.attributes.begin()->second].count;
      const core::uint32 vertexOffset = geometry->m_vertices.size();
      geometry->m_vertices.resize(vertexOffset + meshVerticesSize);

      core::vector<core::uint32> meshIndices;
      meshIndices.resize(tinyModel.accessors[tinyPrimitive.indices].count);

      loadMeshAttributeData("POSITION", reinterpret_cast<core::byte*>(&geometry->m_vertices[vertexOffset].position), sizeof(math::Float3), sizeof(scene::Vertex));
      loadMeshAttributeData("TEXCOORD_0", reinterpret_cast<core::byte*>(&geometry->m_vertices[vertexOffset].texcoord), sizeof(math::Float2), sizeof(scene::Vertex));
      loadMeshIndicesData(meshIndices);
      loadMeshlets(geometry, vertexOffset, meshIndices);

      if (tinyPrimitive.material != -1)
      {
        world->setTexture(loadMaterial());
      }

      geometry->m_meshes.push_back(scene::Mesh{
        .vertexOffset = vertexOffset,
        .vertexCount = static_cast<core::uint32>(geometry->m_vertices.size()),
        .lodCount = 1u,
        .lods = core::array<scene::Mesh_Lod, 8> { scene::Mesh_Lod {
          .indexOffset = vertexIndicestOffset,
          .indexCount = static_cast<core::uint32>(geometry->m_vertexIndices.size() - vertexIndicestOffset),
          .meshletOffset = meshletOffset,
          .meshletCount = static_cast<core::uint32>(geometry->m_meshlets.size() - meshletOffset),
        } }
        });
      };

    core::shared_ptr<scene::Texture> texture;

    const math::Matrix nodeToParent = calculateNodeToParent(tinyNode, parentToModel);

    core::shared_ptr<scene::Camera> camera = loadCamera();
    if (camera)
      cameras.push_back(camera);

    loadMesh(texture, geometry);

    for (core::uint32 childNode : tinyNode.children)
    {
      loadScene(tinyModel.nodes[childNode], nodeToParent);
    }
    };

  tinygltf::Scene& tinyScene = tinyModel.scenes[tinyModel.defaultScene];
  for (core::uint32 tinyNodeIndex : tinyScene.nodes)
  {
    const tinygltf::Node& node = tinyModel.nodes[tinyNodeIndex];
    const math::Matrix nodeToParent = calculateNodeToParent(node, math::smatrix::identity());
    loadScene(tinyModel.nodes[tinyNodeIndex], nodeToParent);
  }
  world->setGeometry(geometry);
  if (!cameras.empty())
    world->setCamera(cameras.front());

  return world;
}

}