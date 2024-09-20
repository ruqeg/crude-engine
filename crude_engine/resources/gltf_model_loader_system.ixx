module;

#define TINYGLTF_NOEXCEPTION
#include <tiny_gltf.h>
#include <DirectXMath.h>
#include <flecs.h>

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

void loadModelToNodeFromFile(flecs::entity node, const char* path, core::shared_ptr<graphics::Command_Pool> commandPool);
bool loadModelFromFile(tinygltf::Model& tinyModel, const char* path);
core::shared_ptr<graphics::Sampler> parseSampler(core::shared_ptr<const graphics::Device> device, const tinygltf::Sampler& tinySampler);
core::shared_ptr<graphics::Image> parseImage(core::shared_ptr<graphics::Command_Pool> commandPool, const tinygltf::Image& tinyImage);
scene::Point_Light_CPU parsePointLight(const tinygltf::Light& tinyLight);
flecs::entity parseNode(tinygltf::Model&                                              tinyModel,
                        flecs::world                                                  world,
                        const tinygltf::Node&                                         tinyNode, 
                        flecs::entity_view                                            parent, 
                        const core::vector<scene::Point_Light_CPU>&                   pointLights,
                        const core::vector<core::shared_ptr<scene::Mesh>>&            meshes, 
                        const core::vector<core::shared_ptr<graphics::Mesh_Buffer>>&  meshBuffers);
core::vector<scene::Vertex> loadVerticesFromPrimitive(tinygltf::Model& tinyModel, const tinygltf::Primitive& tinyPrimitive);
core::vector<core::uint32> loadVertexIndicesFromPrimitive(tinygltf::Model& tinyModel, const tinygltf::Primitive& tinyPrimitive);
void loadBufferFromAccessor(tinygltf::Model& tinyModel, const tinygltf::Accessor& tinyAccessor, core::byte* data, core::size_t elementSize, core::size_t byteStride);
void buildMeshlets(const core::vector<scene::Vertex>&  submeshVertices, 
                   const core::vector<core::uint32>&   submeshVertexIndices,
                   core::vector<core::uint32>&         meshVertexIndices, 
                   core::vector<core::uint8>&          meshPrimitiveIndices,
                   core::vector<scene::Meshlet>&       meshMeshlets);

}