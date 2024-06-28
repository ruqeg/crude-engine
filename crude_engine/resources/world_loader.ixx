module;

#define TINYGLTF_NOEXCEPTION

#include <tiny_gltf.h>

export module crude.resources.world_loader;

export import crude.scene.world;
export import crude.core.optional;
import crude.core.logger;

namespace crude::resources
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

math::Vector loadTranslation(const tinygltf::Node& node)
{
  if (node.translation.size() == 0u)
    return math::svector::zero();

  return math::svector::set(node.translation[0], node.translation[1], node.translation[2], 0.f);
}

math::Vector loadRotation(const tinygltf::Node& node)
{
  if (node.rotation.size() == 0u)
    return math::svector::zero();

  return math::svector::set(node.rotation[0], node.rotation[1], node.rotation[2], 0.f);
}

scene::Camera loadCamera(const tinygltf::Node& tNode, const tinygltf::Camera& tCamera)
{
  scene::Camera camera;
  camera.setName(reinterpret_cast<const char8_t*>(tCamera.name.c_str()));
  camera.setPosition(loadTranslation(tNode));
  camera.setRotation(loadRotation(tNode));
  if (tCamera.type == "perspective")
  {
    camera.calculateViewToClipMatrix(tCamera.perspective.yfov, tCamera.perspective.aspectRatio, tCamera.perspective.znear, tCamera.perspective.zfar);
  }
  return camera;
}

scene::Mesh loadMesh(const tinygltf::Model& tModel, const tinygltf::Node& tNode, const tinygltf::Mesh& tMesh)
{
  scene::Mesh mesh;
  mesh.setName(reinterpret_cast<const char8_t*>(tMesh.name.c_str()));
  for (const tinygltf::Primitive& tPrimitive : tMesh.primitives)
  {
    //positions.resize(tPositionAccessor.count);
    /* fastgltf::iterateAccessorWithIndex<glm::vec3>(model,
      positionAccessor,
      [&](glm::vec3 position, std::size_t idx) { positions[idx] = position; });

    std::vector<glm::vec3> normals;
    auto& normalAccessor = model.accessors[primitive.findAttribute("NORMAL")->second];
    normals.resize(normalAccessor.count);
    fastgltf::iterateAccessorWithIndex<glm::vec3>(model,
      normalAccessor,
      [&](glm::vec3 normal, std::size_t idx) { normals[idx] = normal; });*/

    //std::vector<glm::vec2> texcoords;

    //// Textureless meshes will use factors instead of textures
    //if (primitive.findAttribute("TEXCOORD_0") != primitive.attributes.end())
    //{
    //  auto& texcoordAccessor = model.accessors[primitive.findAttribute("TEXCOORD_0")->second];
    //  texcoords.resize(texcoordAccessor.count);
    //  fastgltf::iterateAccessorWithIndex<glm::vec2>(model,
    //    texcoordAccessor,
    //    [&](glm::vec2 texcoord, std::size_t idx)
    //    { texcoords[idx] = texcoord; });
    //}
    //else
    //{
    //  // If no texcoord attribute, fill with empty texcoords to keep everything consistent and happy
    //  texcoords.resize(positions.size(), {});
    //}

    //FWOG_ASSERT(positions.size() == normals.size() && positions.size() == texcoords.size());

    //std::vector<Vertex> vertices;
    //vertices.resize(positions.size());

    //for (size_t i = 0; i < positions.size(); i++)
    //{
    //  vertices[i] = { positions[i], glm::packSnorm2x16(float32x3_to_oct(normals[i])), texcoords[i] };
    //}

    //return vertices;
  }
  return {};
}

}

export namespace crude::resources
{

core::Optional<core::shared_ptr<scene::World>> loadWorld(const char* path) noexcept
{
  tinygltf::Model tModel;
  if (!loadBinaryFromFile(path, tModel))
  {
    return core::nullopt;
  }

  core::vector<scene::Camera> cameras;
  core::vector<scene::Mesh> meshes;
  for (const auto& node : tModel.nodes)
  {
    if (node.camera != -1)
    {
      cameras.push_back(loadCamera(node, tModel.cameras[node.camera]));
    }
    if (node.mesh != -1)
    {
      meshes.push_back(loadMesh(tModel, node, tModel.meshes[node.mesh]));
    }
  }
  return core::allocateShared<scene::World>(cameras);
}

}