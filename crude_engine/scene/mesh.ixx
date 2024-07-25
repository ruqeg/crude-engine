module;

#include <directxmath/DirectXMath.h>
#include <typeindex>

export module crude.scene.mesh;

export import crude.scene.image;
export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;
export import crude.graphics.storage_buffer;
export import crude.graphics.buffer_descriptor;

export namespace crude::scene
{

struct Meshlet
{
  core::uint8  vertexCount;
  core::uint8  primitiveCount;
  core::uint32   vertexOffset;
  core::uint32   primitiveOffest;
};

struct Vertex
{
  DirectX::XMFLOAT3A position;
  DirectX::XMFLOAT3A normal;
  DirectX::XMFLOAT2A texcoord;
};

struct Sub_Mesh_Lod
{
  core::uint32  indexOffset;
  core::uint32  indexCount;
  core::uint32  meshletOffset;
  core::uint32  meshletCount;
};

struct Sub_Mesh
{
  core::uint32                  vertexOffset;
  core::uint32                  vertexCount;
  core::uint32                  lodCount;
  core::array<Sub_Mesh_Lod, 8>  lods;
};

struct Mesh
{
  core::vector<Vertex>        vertices;
  core::vector<core::uint32>  vertexIndices;
  core::vector<Meshlet>       meshlets;
  core::vector<core::uint8>   primitiveIndices;
  core::vector<Sub_Mesh>      subMeshes;
};

struct Mesh_Buffer
{
  core::shared_ptr<graphics::Storage_Buffer>  verticesBuffer;
  core::shared_ptr<graphics::Storage_Buffer>  meshletsBuffer;
  core::shared_ptr<graphics::Storage_Buffer>  primitiveIndicesBuffer;
  core::shared_ptr<graphics::Storage_Buffer>  vertexIndicesBuffer;
};

}