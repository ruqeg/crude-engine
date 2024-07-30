#include <vulkan/vulkan.hpp>

module crude.graphics.mesh_buffer;

import crude.graphics.device;
import crude.graphics.command_buffer;

namespace crude::graphics
{

Mesh_Buffer::Mesh_Buffer(core::shared_ptr<Command_Buffer> commandBuffer, const scene::Mesh& mesh)
{
  m_submeshesDrawsBuffer = core::allocateShared<graphics::Storage_Buffer>(commandBuffer, core::vector<scene::Sub_Mesh_Draw>(mesh.submeshes.begin(), mesh.submeshes.end()));
  m_verticesBuffer = core::allocateShared<graphics::Storage_Buffer>(commandBuffer, mesh.vertices);
  m_meshletsBuffer = core::allocateShared<graphics::Storage_Buffer>(commandBuffer, mesh.meshlets);
  m_primitiveIndicesBuffer = core::allocateShared<graphics::Storage_Buffer>(commandBuffer, mesh.primitiveIndices);
  m_vertexIndicesBuffer = core::allocateShared<graphics::Storage_Buffer>(commandBuffer, mesh.vertexIndices);
}

}