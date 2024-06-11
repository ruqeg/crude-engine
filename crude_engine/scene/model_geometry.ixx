module;

export module crude.scene.model_geometry;

export import crude.scene.mesh;

export namespace crude::scene
{

struct Mesh_Range
{
  core::uint32 vertexOffset;
  core::uint32 indexOffset;
  core::uint32 vertexNum;
  core::uint32 indexNum;
};

class Model_Geometry
{
public:
  Model_Geometry() = default;
  ~Model_Geometry() = default;
  Model_Geometry(const core::vector<Mesh>& meshes, const core::vector<Mesh_Range>& ranges) noexcept
    : m_meshes(meshes), m_ranges(ranges) {}
public:
  void setMeshes(const core::vector<Mesh>& meshes) { m_meshes = meshes; }
  void setRanges(const core::vector<Mesh_Range>& ranges) { m_ranges = ranges; }
  const core::vector<Mesh>& getMeshes() const { return m_meshes; }
  const core::vector<Mesh_Range>& getRanges() const { return m_ranges; }
  core::vector<Mesh>& getMeshes() { return m_meshes; }
  core::vector<Mesh_Range>& getRanges() { return m_ranges; }
protected:
  core::vector<Mesh>        m_meshes;
  core::vector<Mesh_Range>  m_ranges;
};

}