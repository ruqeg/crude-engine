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
  Model_Geometry(const core::span<const Mesh>& meshes, const core::span<const Mesh_Range>& ranges) noexcept
    : m_meshes(meshes.begin(), meshes.end()), m_ranges(ranges.begin(), ranges.end()) {}
public:
  void setMeshes(const core::span<const Mesh>& meshes) { m_meshes.assign(meshes.begin(), meshes.end()); }
  void setRanges(const core::span<const Mesh_Range>& ranges) { m_ranges.assign(ranges.begin(), ranges.end()); }
  const core::vector<Mesh>& getMeshes() const { return m_meshes; }
  const core::vector<Mesh_Range>& getRanges() const { return m_ranges; }
  core::vector<Mesh>& getMeshes() { return m_meshes; }
  core::vector<Mesh_Range>& getRanges() { return m_ranges; }
protected:
  core::vector<Mesh>        m_meshes;
  core::vector<Mesh_Range>  m_ranges;
};

}