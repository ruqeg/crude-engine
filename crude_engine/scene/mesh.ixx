module;

#include <directxmath/DirectXMath.h>

export module crude.scene.mesh;

export import crude.scene.image;
export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;

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

struct Mesh_Lod
{
  core::uint32  indexOffset;
  core::uint32  indexCount;
  core::uint32  meshletOffset;
  core::uint32  meshletCount;
};

struct Mesh
{
  core::uint32              vertexOffset;
  core::uint32              vertexCount;
  core::uint32              lodCount;
  core::array<Mesh_Lod, 8>  lods;
};

struct Geometry
{
  core::vector<Vertex>               m_vertices;
  core::vector<core::uint32>         m_vertexIndices;
  core::vector<Meshlet>              m_meshlets;
  core::vector<core::uint8>          m_primitiveIndices;
  core::vector<Mesh>                 m_meshes;
};

struct Node
{
  core::uint32                         m_geometryIndex;
  core::uint32                         m_textureIndex;
  DirectX::XMFLOAT4X4                  m_nodeToParent;
  core::vector<core::shared_ptr<Node>> m_nodes;
};

}