module;

export module crude.scene.mesh;

export import crude.scene.image;
export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;
export import crude.math.fuicont;
export import crude.math.matrix;

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
  math::Float3A position;
  math::Float3A normal;
  math::Float2A texcoord;
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

class Triangle_Index
{
public:
  Triangle_Index() : m_i0(0), m_i1(0), m_i2(0) {};
  explicit Triangle_Index(core::uint32 i0, core::uint32 i1, core::uint32 i2) : m_i0(i0), m_i1(i1), m_i2(i2) {}
  void seti0(core::uint32 i0) noexcept { m_i0 = i0; }
  void seti1(core::uint32 i1) noexcept { m_i1 = i1; }
  void seti2(core::uint32 i2) noexcept { m_i2 = i2; }
  void set(core::uint32 i0, core::uint32 i1, core::uint32 i2) noexcept { m_i0 = i0; m_i1 = i1; m_i2 = i2; }
  inline core::uint32 geti0() const noexcept { return m_i0; };
  inline core::uint32 geti1() const noexcept { return m_i1; };
  inline core::uint32 geti2() const noexcept { return m_i2; };
private:
  core::uint32 m_i0;
  core::uint32 m_i1;
  core::uint32 m_i2;
};

class alignas(4) Triangle_Index_Packed8
{
public:
  Triangle_Index_Packed8() : m_bytes(0) {};
  explicit Triangle_Index_Packed8(core::uint8 i0, core::uint8 i1, core::uint8 i2) : m_bytes{ i0 | (i1 << 8) | (i2 << 16) } {}
  void seti0(core::uint8 i0) noexcept { m_bytes = i0 | (m_bytes & 0xffffff00); }
  void seti1(core::uint8 i1) noexcept { m_bytes = i1 | (m_bytes & 0xffff00ff); }
  void seti2(core::uint8 i2) noexcept { m_bytes = i2 | (m_bytes & 0xff00ffff); }
  void set(core::uint8 i0, core::uint8 i1, core::uint8 i2) noexcept { m_bytes = i0 | (i1 << 8) | (i2 << 16); }
  inline core::uint8 geti0() const noexcept { return m_bytes & 0xff; };
  inline core::uint8 geti1() const noexcept { return (m_bytes >> 8) & 0xff; };
  inline core::uint8 geti2() const noexcept { return (m_bytes >> 16) & 0xff; };
private:
  core::int32 m_bytes;
};

struct Geometry
{
  core::vector<Vertex>                  m_vertices;
  core::vector<core::uint32>            m_vertexIndices;
  core::vector<Meshlet>                 m_meshlets;
  core::vector<core::uint8>             m_primitiveIndices;
  core::vector<Mesh>                    m_meshes;
  core::vector<math::Matrix>            m_meshToModel;
};

}