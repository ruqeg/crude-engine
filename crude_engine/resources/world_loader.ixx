module;

#define TINYGLTF_NOEXCEPTION
#include <tiny_gltf.h>

export module crude.resources.world_loader;

export import crude.scene.world;
export import crude.core.optional;
import crude.core.logger;

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

core::Optional<core::shared_ptr<scene::World>> loadWorld(const char* path) noexcept
{
  static_assert(sizeof(char) == sizeof(core::char8));
  tinygltf::Model tinyModel;
  tinygltf::TinyGLTF tinyLoader;
  std::string err;
  std::string warn;

  // !TODO
  const bool loadResult = tinyLoader.LoadBinaryFromFile(&tinyModel, &err, &warn, path);

  if (!warn.empty())
  {
    core::logWarning(core::Debug::Channel::FileIO, "Warning while loading model geometry: %s", warn.c_str());
  }

  if (!err.empty())
  {
    core::logError(core::Debug::Channel::FileIO, "Error while loading model geometry: %s", err.c_str());
    return core::nullopt;
  }

  if (!loadResult)
  {
    core::logError(core::Debug::Channel::FileIO, "Failed to parse glTF");
    return core::nullopt;
  }

  core::shared_ptr<scene::World> world = core::allocateShared<scene::World>();
  for (const auto& tinyNode : tinyModel.nodes)
  {
    if (tinyNode.camera != -1)
    {
      core::shared_ptr<scene::Camera> camera = core::allocateShared<scene::Camera>();
      // !TODO
      const tinygltf::Camera& tinyCamera = tinyModel.cameras[tinyNode.camera];
      camera->setName(reinterpret_cast<const char8_t*>(tinyCamera.name.c_str()));
      camera->setPosition(loadTinyTranslation(tinyNode));
      camera->setRotation(loadTinyRotation(tinyNode));
      if (tinyCamera.type == "perspective")
      {
        camera->calculateViewToClipMatrix(tinyCamera.perspective.yfov, tinyCamera.perspective.aspectRatio, tinyCamera.perspective.znear, tinyCamera.perspective.zfar);
      }
      world->setCamera(camera);
    }
    if (tinyNode.mesh != -1)
    {
      core::shared_ptr<scene::Mesh> mesh = core::allocateShared<scene::Mesh>();
      const tinygltf::Mesh& tinyMesh = tinyModel.meshes[tinyNode.mesh];

      const tinygltf::Accessor& accessor = tinyModel.accessors[tinyMesh.primitives.front().attributes.at("POSITION")];
      const tinygltf::BufferView& bufferView = tinyModel.bufferViews[accessor.bufferView];
      const tinygltf::Buffer& buffer = tinyModel.buffers[bufferView.buffer];
      
      // !TODO
      std::vector<scene::Vertex> vertices(accessor.count);

      const float* positions = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
      for (size_t i = 0; i < accessor.count; ++i)
      {
        core::logError(core::Debug::Channel::FileIO, "positions[%i] = {%f, %f, %f}", i, positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]);
        vertices[i].position = math::Float3A(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]);
      }  
     /* vertices = {
        scene::Vertex{ math::Float3A{-0.5f, -0.5f, 0.0f}, math::Float3A{1.0f, 0.0f, 0.0f}, math::Float2A{1.0f, 0.0f}, math::Float3A{}, math::Float3A{}, math::Float3A{} },
        scene::Vertex{ math::Float3A{0.5f, -0.5f, 0.0f}, math::Float3A{0.0f, 1.0f, 0.0f}, math::Float2A{0.0f, 0.0f}, math::Float3A{}, math::Float3A{}, math::Float3A{} },
        scene::Vertex{ math::Float3A{0.5f, 0.5f, 0.0f}, math::Float3A{0.0f, 0.0f, 1.0f}, math::Float2A{0.0f, 1.0f}, math::Float3A{}, math::Float3A{}, math::Float3A{} },
        scene::Vertex{ math::Float3A{-0.5f, 0.5f, 0.0f}, math::Float3A{1.0f, 1.0f, 0.0f}, math::Float2A{1.0f, 1.0f}, math::Float3A{}, math::Float3A{}, math::Float3A{} },

        scene::Vertex{ math::Float3A{-0.5f, -0.5f, -0.5f}, math::Float3A{1.0f, 0.0f, 0.0f}, math::Float2A{1.0f, 0.0f}, math::Float3A{}, math::Float3A{}, math::Float3A{} },
        scene::Vertex{ math::Float3A{0.5f, -0.5f, -0.5f}, math::Float3A{0.0f, 1.0f, 0.0f}, math::Float2A{0.0f, 0.0f}, math::Float3A{}, math::Float3A{}, math::Float3A{} },
        scene::Vertex{ math::Float3A{0.5f, 0.5f, -0.5f}, math::Float3A{0.0f, 0.0f, 1.0f}, math::Float2A{0.0f, 1.0f}, math::Float3A{}, math::Float3A{}, math::Float3A{} },
        scene::Vertex{ math::Float3A{-0.5f, 0.5f, -0.5f}, math::Float3A{1.0f, 1.0f, 0.0f}, math::Float2A{1.0f, 1.0f}, math::Float3A{}, math::Float3A{}, math::Float3A{} }
      };*/
      mesh->setVertices(vertices);




      const tinygltf::Accessor& accessorInd = tinyModel.accessors[tinyMesh.primitives.front().indices];
      const tinygltf::BufferView& bufferViewInd = tinyModel.bufferViews[accessorInd.bufferView];
      const tinygltf::Buffer& bufferInd = tinyModel.buffers[bufferViewInd.buffer];

      // !TODO
      std::vector<scene::Triangle_Index> triangleIndices(accessorInd.count / 3);

      const uint16_t* indices = reinterpret_cast<const uint16_t*>(&bufferInd.data[bufferViewInd.byteOffset + accessorInd.byteOffset]);
      for (size_t i = 0; i < accessorInd.count / 3; ++i)
      {
        triangleIndices[i].set(indices[i * 3 + 0], indices[i * 3 + 1], indices[i * 3 + 2]);
        core::logError(core::Debug::Channel::FileIO, "triangleIndices[%i]: %i %i %i", i, triangleIndices[i].geti0(), triangleIndices[i].geti1(), triangleIndices[i].geti2());
      }
      /*triangleIndices.resize(4);
      triangleIndices[0].set(0, 1, 2);
      triangleIndices[1].set(2, 3, 0);
      triangleIndices[2].set(4, 5, 6);
      triangleIndices[3].set(6, 7, 4);*/
      mesh->setTriangleIndices(triangleIndices);


      world->setMesh(mesh);
    }
  }
  return world;
}

}