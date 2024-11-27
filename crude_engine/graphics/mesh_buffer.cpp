#include <vulkan/vulkan.hpp>

module crude.gfx.mesh_buffer;

import crude.gfx.vk.device;
import crude.gfx.vk.command_buffer;

namespace crude::gfx
{

Mesh_Buffer::Mesh_Buffer(core::shared_ptr<vk::Command_Buffer> commandBuffer, const scene::Mesh& mesh)
{
  m_submeshesDrawsBuffer = core::allocateShared<vk::Storage_Buffer>(commandBuffer, core::vector<scene::Sub_Mesh_Draw>(mesh.submeshes.begin(), mesh.submeshes.end()));
  m_verticesBuffer = core::allocateShared<vk::Storage_Buffer>(commandBuffer, mesh.vertices);
  m_meshletsBuffer = core::allocateShared<vk::Storage_Buffer>(commandBuffer, mesh.meshlets);
  m_primitiveIndicesBuffer = core::allocateShared<vk::Storage_Buffer>(commandBuffer, mesh.primitiveIndices);
  m_vertexIndicesBuffer = core::allocateShared<vk::Storage_Buffer>(commandBuffer, mesh.vertexIndices);
}

}