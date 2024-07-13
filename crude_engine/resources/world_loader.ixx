module;

#define TINYGLTF_NOEXCEPTION
#include <tiny_gltf.h>
#include <algorithm>
#include <functional>

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

// !TODO

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
  tinygltf::Scene& tinyScene = tinyModel.scenes[tinyModel.defaultScene];
  math::Matrix meshToModel = math::smatrix::identity();

  std::function<void(tinygltf::Node&)> loadScene; 
  loadScene = [&](tinygltf::Node& tinyNode) {
    if (tinyNode.matrix.size() != 0)
      meshToModel =  math::Matrix(
        tinyNode.matrix[0], tinyNode.matrix[1], tinyNode.matrix[2], tinyNode.matrix[3],
        tinyNode.matrix[4], tinyNode.matrix[5], tinyNode.matrix[6], tinyNode.matrix[7],
        tinyNode.matrix[8], tinyNode.matrix[9], tinyNode.matrix[10], tinyNode.matrix[11],
        tinyNode.matrix[12], tinyNode.matrix[13], tinyNode.matrix[14], tinyNode.matrix[15]);



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

      const tinygltf::Primitive primitive = tinyMesh.primitives.front();
      const tinygltf::Accessor& accessor = tinyModel.accessors[primitive.attributes.at("POSITION")];
      const tinygltf::BufferView& bufferView = tinyModel.bufferViews[accessor.bufferView];
      const tinygltf::Buffer& buffer = tinyModel.buffers[bufferView.buffer];

      // !TODO
      std::vector<scene::Vertex> vertices(accessor.count);

      const float* positions = reinterpret_cast<const float*>(&buffer.data[0]);
      for (size_t i = 0; i < accessor.count; ++i)
      {
        vertices[i].position = math::Float3A(
          positions[3 + i * 6 + 0],
          positions[3 + i * 6 + 1],
          positions[3 + i * 6 + 2]);

      }


      const tinygltf::Accessor& uaccessor = tinyModel.accessors[primitive.attributes.at("TEXCOORD_0")];
      const tinygltf::BufferView& ubufferView = tinyModel.bufferViews[uaccessor.bufferView];
      const tinygltf::Buffer& ubuffer = tinyModel.buffers[ubufferView.buffer];
      const float* uvo = reinterpret_cast<const float*>(&ubuffer.data[0] + ubufferView.byteOffset);
      for (size_t i = 0; i < uaccessor.count; ++i)
      {
        vertices[i].texcoord = math::Float2A(
          uvo[i * 2 + 0],
          uvo[i * 2 + 1]);
      }


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
      }
      mesh->setTriangleIndices(triangleIndices);
      mesh->setMeshToModel(meshToModel);

      world->setMesh(mesh);


      const tinygltf::Material& material = tinyModel.materials[primitive.material];
      const tinygltf::Texture& texture = tinyModel.textures[material.pbrMetallicRoughness.baseColorTexture.index];
      const tinygltf::Image& image = tinyModel.images[texture.source];
      core::shared_ptr<core::byte[]> imageByte = core::allocateShared<core::byte[]>(image.image.size());
      std::memcpy(imageByte.get(), image.image.data(), static_cast<core::size_t>(image.image.size()));

      core::shared_ptr<scene::Image> mimage = core::allocateShared<scene::Image>(imageByte, scene::Image_Format::IMAGE_FORMAT_RGB_ALPHA, image.width, image.height);
      core::shared_ptr<graphics::Sampler_State> samplerState = core::allocateShared<graphics::Sampler_State>(graphics::csamlper_state::gMagMinMipLinearRepeat);
      core::shared_ptr<scene::Texture> ttexture = core::allocateShared<scene::Texture>(mimage, samplerState);
      world->setTexture(ttexture);
    }




    for (core::uint32 childNode : tinyNode.children)
    {
      loadScene(tinyModel.nodes[childNode]);
    }
  };

  for (const auto& tinyNode : tinyScene.nodes)
  {
    loadScene(tinyModel.nodes[tinyNode]);
  }
  return world;
}

}