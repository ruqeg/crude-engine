module;

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION

#include <tiny_gltf.h>

export module crude.resources.world_loader;

export import crude.scene.world;
export import crude.core.optional;
import crude.core.logger;

namespace crude::resources
{

namespace helper
{
bool loadBinaryFromFile(const char* path, tinygltf::Model& model)
{
  static_assert(sizeof(char) == sizeof(core::char8));
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
}

scene::Camera loadCamera(const tinygltf::Model& model, const tinygltf::Node& node)
{
  const tinygltf::Camera& tCamera = model.cameras[node.camera];
  
  scene::Camera camera;
  camera.setName(reinterpret_cast<const char8_t*>(tCamera.name.c_str()));
  if (!node.translation.empty())
  {
    camera.setPosition(node.translation[0], node.translation[1], node.translation[2]);
  }
  if (!node.rotation.empty())
  {
    camera.setRotation(node.rotation[0], node.rotation[1], node.rotation[2]);
  }
  if (tCamera.type == "perspective")
  {
    camera.calculateViewToClipMatrix(tCamera.perspective.yfov, tCamera.perspective.aspectRatio, tCamera.perspective.znear, tCamera.perspective.zfar);
  }
  return camera;
}

}

}

export namespace crude::resources
{

core::Optional<core::shared_ptr<scene::World>> loadWorld(const char* path) noexcept
{
  tinygltf::Model tModel;
  if (!helper::loadBinaryFromFile(path, tModel))
  {
    return core::nullopt;
  }

  core::vector<scene::Camera> cameras;
  for (const auto& node : tModel.nodes)
  {
    if (node.camera != -1)
    {
      cameras.push_back(helper::loadCamera(tModel, node));
    }
  }
  return core::allocateShared<scene::World>(cameras);
}

}