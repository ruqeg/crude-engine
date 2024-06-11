module;

export module crude.scene.mesh;

export import crude.scene.vertex;
export import crude.scene.index;
export import crude.core.std_containers_heap;

export namespace crude::scene
{

class Mesh
{
public:
  Mesh() = default;
  ~Mesh() = default;
public:
  void setVertices(const core::vector<Vertex_CPU>& vertices) { m_vertices = vertices; }
  void setIndices(const core::vector<Index_Triangle_CPU>& indices) { m_indices = indices; }
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