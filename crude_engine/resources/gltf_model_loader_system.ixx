module;

#define TINYGLTF_NOEXCEPTION
#include <tiny_gltf.h>
#include <DirectXMath.h>
#include <flecs.h>
#include <vulkan/vulkan.h>
#include <filesystem>

export module crude.resources.gltf_model_loader_system;

export import crude.scene.mesh;
export import crude.scene.light;
export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;

export namespace crude::graphics
{

class Device;
class Sampler;
class Command_Pool;
class Image;
class Mesh_Buffer;
class Texture;
class Image_View;

}

export namespace crude::resources
{

struct GLTF_Model_Loader_Uninitialized_Flag
{
};

struct GLTF_Model_Metadata_Component
{
  core::string path;
};

struct GLTF_Model_Loader_Context
{
  core::shared_ptr<graphics::Command_Pool> transferCommandPool;
};

[[nodiscard]] flecs::system registerGLTFModelLoaderSystem(flecs::world world, core::shared_ptr<GLTF_Model_Loader_Context> ctx);

}

namespace crude::resources
{

// 0 - crude::resources::GLTF_Model_Metadata_Component
// 1 - crude::resources::GLTF_Model_Loader_Uninitialized_Flag
void gltfModelLoaderSystemProcess(flecs::iter& it);

class GLTF_Loader
{
public:
  GLTF_Loader(core::shared_ptr<graphics::Command_Pool> commandPool);
public:
  void loadToNodeFromFile(flecs::entity node, const std::filesystem::path& path);
private:
  bool loadModelFromFile(const std::filesystem::path& path);
  scene::Point_Light_CPU parsePointLight(const tinygltf::Light& tinyLight);
  core::shared_ptr<graphics::Material> parseMaterial(core::int32 tinyMaterialIndex);
  core::shared_ptr<graphics::Image_View> parseImageView(const core::int32 tinyImageIndex, VkFormat format, VkFilter mipmapFilter = VK_FILTER_LINEAR);
  core::shared_ptr<graphics::Texture> parseTexture(const core::int32 tinyTextureIndex, const VkFormat format, core::span<const core::byte> texelForUnitialized);
  flecs::entity parseNode(flecs::world world, const tinygltf::Node& tinyNode);
private:
  core::vector<scene::Vertex> loadVerticesFromPrimitive(const tinygltf::Primitive& tinyPrimitive);
  core::vector<core::uint32> loadVertexIndicesFromPrimitive(const tinygltf::Primitive& tinyPrimitive);
  void loadBufferFromAccessor(const tinygltf::Accessor& tinyAccessor, core::byte* data, core::size_t elementSize, core::size_t byteStride);
  void buildMeshlets(const core::vector<scene::Vertex>&  submeshVertices, 
                     const core::vector<core::uint32>&   submeshVertexIndices,
                     core::vector<core::uint32>&         meshVertexIndices, 
                     core::vector<core::uint8>&          meshPrimitiveIndices,
                     core::vector<scene::Meshlet>&       meshMeshlets);
private:
  tinygltf::Model                                        m_tinyModel;
  core::shared_ptr<graphics::Command_Pool>               m_commandPool;

  core::vector<scene::Point_Light_CPU>                   m_pointLights;
  core::vector<core::shared_ptr<scene::Mesh>>            m_meshes;
  core::vector<core::shared_ptr<graphics::Mesh_Buffer>>  m_meshBuffers;
  core::shared_ptr<graphics::Sampler>                    m_sampler;
  core::vector<core::shared_ptr<graphics::Image_View>>   m_imageViews;
  core::vector<core::shared_ptr<graphics::Texture>>      m_textures;

  static constexpr VkExtent2D                            cTextureExtentLimit{ .width = 2048, .height = 2048 };
};

}