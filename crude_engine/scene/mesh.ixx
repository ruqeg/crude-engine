module;

#include <DirectXMath.h>
#include <typeindex>

export module crude.scene.mesh;

export import crude.scene.image;
export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;

export namespace crude::graphics
{
class Material;
}

export namespace crude::scene
{

struct Meshlet
{
  core::uint8   vertexCount;
  core::uint8   primitiveCount;
  core::uint32  vertexOffset;
  core::uint32  primitiveOffest;
};

struct Vertex
{
  DirectX::XMFLOAT3A position;
  DirectX::XMFLOAT3A normal;
  DirectX::XMFLOAT3A tangent;
  DirectX::XMFLOAT3A bitangent;
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
  core::uint32                          vertexOffset;
  core::uint32                          vertexCount;
  core::uint32                          lodCount;
  core::array<Sub_Mesh_Lod, 8>          lods;
  core::shared_ptr<graphics::Material>  material;
};

struct Sub_Mesh_Draw
{
  Sub_Mesh_Draw(const Sub_Mesh& submesh)
    : vertexOffset(submesh.vertexOffset)
    , vertexCount(submesh.vertexCount)
    , lodCount(submesh.lodCount)
    , lods(submesh.lods) {}
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
  core::vector<Sub_Mesh>      submeshes;
};

}