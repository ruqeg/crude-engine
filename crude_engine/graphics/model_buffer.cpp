#include <vulkan/vulkan.hpp>

module crude.graphics.model_buffer;

import crude.graphics.index_buffer;
import crude.graphics.vertex_buffer;
import crude.graphics.command_buffer;
import crude.graphics.device;
import crude.graphics.fence;
import crude.graphics.vertex_gpu;
import crude.graphics.index_gpu;

namespace crude::graphics
{

Model_Buffer::Model_Buffer(core::shared_ptr<Command_Buffer> commandBuffer,
                           const scene::Model_Geometry&     modelGeometry)
{
  core::uint32 modelVerticesNum = 0u;
  core::uint32 modelIndicesNum = 0u;
  for (const scene::Mesh_Range& meshRange : modelGeometry.getRanges())
  {
    modelVerticesNum += meshRange.vertexNum;
    modelIndicesNum += meshRange.indexNum;
  }

  core::vector<Vertex_GPU> modelGpuVertices(modelVerticesNum);
  core::vector<Index_Triangle_GPU> modelGpuIndices(modelIndicesNum / 3);
  core::uint32 modelGpuVertexInx = 0u;
  core::uint32 modelGpuIndexInx = 0u;
  for (const scene::Mesh& mesh : modelGeometry.getMeshes())
  {
    for (const scene::Vertex_CPU vertexCPU : mesh.getVertices())
    {
      modelGpuVertices[modelGpuVertexInx++] = Vertex_GPU(vertexCPU);
    }
    for (const scene::Index_Triangle_CPU indexCPU : mesh.getIndices())
    {
      modelGpuIndices[modelGpuIndexInx++] = Index_Triangle_GPU(indexCPU);
    }
  }

  m_vertexBuffer = core::allocateShared<Vertex_Buffer>(commandBuffer, modelGpuVertices);
  m_indexBuffer = core::allocateShared<Index_Buffer>(commandBuffer, Index_Triangle_GPU::getType(), modelGpuIndices);
}

core::shared_ptr<Vertex_Buffer> Model_Buffer::getVertexBuffer()
{
  return m_vertexBuffer;
}

core::shared_ptr<Index_Buffer> Model_Buffer::getIndexBuffer()
{
  return m_indexBuffer;
}

}
