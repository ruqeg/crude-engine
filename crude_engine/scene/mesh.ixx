module;

export module crude.scene.mesh;

export import crude.scene.vertex;
export import crude.scene.index;
export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;

export namespace crude::scene
{

class Mesh
{
public:
  Mesh() = default;
  ~Mesh() = default;
  Mesh(const core::span<const Vertex_CPU>& vertices, const core::span<const Index_Triangle_CPU>& indices)
    : m_vertices(vertices.begin(), vertices.end()), m_indices(indices.begin(), indices.end()) {}
public:
  void setVertices(const core::span<const Vertex_CPU>& vertices) { m_vertices.assign(vertices.begin(), vertices.end()); }
  void setIndices(const core::span<const Index_Triangle_CPU>& indices) { m_indices.assign(indices.begin(), indices.end()); }
  core::vector<Vertex_CPU>& getVertices() { return m_vertices; }
  core::vector<Index_Triangle_CPU>& getIndices() { return m_indices; }
  const core::vector<Vertex_CPU>& getVertices() const { return m_vertices; }
  const core::vector<Index_Triangle_CPU>& getIndices() const { return m_indices; }
  core::uint32 getVerticesNum() const { return m_vertices.size(); }
  core::uint32 getIndicesNum() const { return m_indices.size(); }
private:
  core::vector<Vertex_CPU> m_vertices;
  core::vector<Index_Triangle_CPU> m_indices;
};

}