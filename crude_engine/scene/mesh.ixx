module;

export module crude.scene.mesh;

export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;
export import crude.math.fuicont;
export import crude.math.matrix;

export namespace crude::scene
{

struct  Vertex
{
  math::Float3A position;
  math::Float3A tmpcolor;
  math::Float2A texcoord;
  math::Float3A normal;
  math::Float3A tangent;
  math::Float3A bitangent;
};

struct alignas(4) Triangle_Index
{
public:
  Triangle_Index() : m_bytes(0) {};
  explicit Triangle_Index(core::uint16 i0, core::uint16 i1, core::uint16 i2)
    : m_bytes{ i0 | (i1 << 10) | (i2 << 20) } {}
  void set(core::uint16 i0, core::uint16 i1, core::uint16 i2) noexcept
  {
    m_bytes = i0 | (i1 << 10) | (i2 << 20);
  }
  inline core::uint16 geti0() const noexcept { return m_bytes & 0x3ff; };
  inline core::uint16 geti1() const noexcept { return (m_bytes >> 10) & 0x3ff; };
  inline core::uint16 geti2() const noexcept { return (m_bytes >> 20) & 0x3ff; };
private:
  core::int32 m_bytes;
};

class Mesh
{
public:
  Mesh() = default;
  ~Mesh() = default;
  Mesh(const core::vector<Vertex>& vertices, const core::vector<Triangle_Index>& triangleIndices)
    : m_vertices(vertices), m_triangleIndices(triangleIndices) {}
  Mesh(core::span<const Vertex> vertices, core::span<const Triangle_Index> triangleIndices)
    : m_vertices(vertices.begin(), vertices.end()), m_triangleIndices(triangleIndices.begin(), triangleIndices.end()) {}
public:
  void setMeshToModel(math::Matrix meshToModel) { m_meshToModel = meshToModel; }
  void setVertices(core::span<const Vertex> vertices) { m_vertices.assign(vertices.begin(), vertices.end()); }
  void setTriangleIndices(core::span<const Triangle_Index> triangleIndices) { m_triangleIndices.assign(triangleIndices.begin(), triangleIndices.end()); }
  void setVertices(const core::vector<Vertex>& vertices) { m_vertices = vertices; }
  void setTriangleIndices(const core::vector<Triangle_Index>& triangleIndices) { m_triangleIndices = triangleIndices; }
  void setName(const core::u8string& name) { m_name = name; }
  const core::u8string& getName() const { return m_name; }
  const core::vector<Vertex>& getVertices() const { return m_vertices; }
  const core::vector<Triangle_Index>& getTriangleIndices() const { return m_triangleIndices; }
  const math::Matrix& getMeshToModel() const { return m_meshToModel; }
  core::uint32 getVerticesCount() const { return m_vertices.size(); }
  core::uint32 getTriangleIndicesCount() const { return m_triangleIndices.size(); }
private:
  core::vector<Vertex>          m_vertices;
  core::vector<Triangle_Index>  m_triangleIndices;
  core::u8string                m_name;
  math::Matrix                  m_meshToModel;
};

}