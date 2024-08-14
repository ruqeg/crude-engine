module;

#define TINYGLTF_NOEXCEPTION
#include <tiny_gltf.h>
#include <directxmath/DirectXMath.h>
#include <flecs.h>

export module crude.resources.gltf_loader;

export import crude.scene.mesh;
export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;
import crude.core.logger;
import crude.core.assert;

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

class GLTF_Loader
{
public:
  explicit GLTF_Loader(core::shared_ptr<graphics::Command_Pool> commandPool, flecs::world world);
public:
  flecs::entity loadNodeFromFile(const char* path);
  core::shared_ptr<graphics::Sampler> parseSampler(const tinygltf::Sampler& tinySampler);
  core::shared_ptr<graphics::Image> parseImage(const tinygltf::Image& tinyImage);
  flecs::entity parseNode(const tinygltf::Node& tinyNode, flecs::entity_view parent, const core::vector<core::shared_ptr<scene::Mesh>>& meshes, const core::vector<core::shared_ptr<graphics::Mesh_Buffer>>& meshBuffers);
  core::vector<scene::Vertex> loadVerticesFromPrimitive(const tinygltf::Primitive& tinyPrimitive);
  core::vector<core::uint32> loadVertexIndicesFromPrimitive(const tinygltf::Primitive& tinyPrimitive);
  void loadBufferFromAccessor(const tinygltf::Accessor& tinyAccessor, core::byte* data, core::size_t elementSize, core::size_t byteStride);
  void buildMeshlets(const core::vector<scene::Vertex>&  submeshVertices, 
                     const core::vector<core::uint32>&   submeshVertexIndices,
                     core::vector<core::uint32>&         meshVertexIndices, 
                     core::vector<core::uint8>&          meshPrimitiveIndices,
                     core::vector<scene::Meshlet>&       meshMeshlets);

private:
  bool loadModelFromFile(const char* path);
private:
  core::shared_ptr<const graphics::Device>  m_device;
  core::shared_ptr<graphics::Command_Pool>  m_commandPool;
  tinygltf::Model                           m_tinyModel;
  flecs::world                              m_world;
};

}