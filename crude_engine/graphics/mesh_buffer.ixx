module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.mesh_buffer;

export import crude.gfx.vk.storage_buffer;
export import crude.scene.mesh;

export namespace crude::gfx
{

class Device;

class Mesh_Buffer
{
public:
  explicit Mesh_Buffer(core::shared_ptr<vk::Command_Buffer> commandBuffer, const scene::Mesh& mesh);
  core::shared_ptr<vk::Storage_Buffer> getSubmeshesDrawsBuffer() { return m_submeshesDrawsBuffer; }
  core::shared_ptr<vk::Storage_Buffer> getVerticesBuffer() { return m_verticesBuffer; }
  core::shared_ptr<vk::Storage_Buffer> getMeshletsBuffer() { return m_meshletsBuffer; }
  core::shared_ptr<vk::Storage_Buffer> getPrimitiveIndicesBuffer() { return m_primitiveIndicesBuffer; }
  core::shared_ptr<vk::Storage_Buffer> getVertexIndicesBuffer() { return m_vertexIndicesBuffer; }
  core::shared_ptr<const vk::Storage_Buffer> getSubmeshesDrawsBuffer() const { return m_submeshesDrawsBuffer; }
  core::shared_ptr<const vk::Storage_Buffer> getVerticesBuffer() const { return m_verticesBuffer; }
  core::shared_ptr<const vk::Storage_Buffer> getMeshletsBuffer() const { return m_meshletsBuffer; }
  core::shared_ptr<const vk::Storage_Buffer> getPrimitiveIndicesBuffer() const { return m_primitiveIndicesBuffer; }
  core::shared_ptr<const vk::Storage_Buffer> getVertexIndicesBuffer() const { return m_vertexIndicesBuffer; }
private:
  core::shared_ptr<vk::Storage_Buffer>  m_submeshesDrawsBuffer;
  core::shared_ptr<vk::Storage_Buffer>  m_verticesBuffer;
  core::shared_ptr<vk::Storage_Buffer>  m_meshletsBuffer;
  core::shared_ptr<vk::Storage_Buffer>  m_primitiveIndicesBuffer;
  core::shared_ptr<vk::Storage_Buffer>  m_vertexIndicesBuffer;
};

// !TODO someday
class RT_Mesh_Buffer
{
public:
  explicit RT_Mesh_Buffer(core::shared_ptr<vk::Command_Buffer> commandBuffer);
};

}