#include <vulkan/vulkan.hpp>

module crude.graphics.model_buffer;
import crude.graphics.command_buffer;
import crude.graphics.device;
import crude.graphics.fence;

namespace crude::graphics
{

Model_Buffer::Model_Buffer(core::Shared_Ptr<Queue> queue, core::Shared_Ptr<Command_Pool> commandPool, const scene::Model_Geometry& modelGeometry)
  :
  m_queue(queue), m_commandPool(commandPool)
{
  core::uint32 modelVerticesNum = 0u;
  core::uint32 modelIndicesNum = 0u;
  for (const scene::Mesh_Range& meshRange : modelGeometry.getRanges())
  {
    modelVerticesNum += meshRange.vertexNum;
    modelIndicesNum += meshRange.indexNum;
  }

  core::vector<scene::Vertex_GPU> modelGpuVertices(modelVerticesNum);
  core::vector<scene::Index_Triangle_GPU> modelGpuIndices(modelIndicesNum);
  core::uint32 modelGpuVertexInx = 0u;
  core::uint32 modelGpuIndexInx = 0u;
  for (const scene::Mesh& mesh : modelGeometry.getMeshes())
  {
    for (const scene::Vertex_CPU vertexCPU : mesh.getVertices())
    {
      modelGpuVertices[modelGpuVertexInx++] = static_cast<scene::Vertex_GPU>(vertexCPU);
    }
    for (const scene::Index_Triangle_CPU indexCPU : mesh.getIndices())
    {
      modelGpuIndices[modelGpuIndexInx++] = static_cast<scene::Index_Triangle_GPU>(indexCPU);
    }
  }
  //initalizeBuffer(m_vertexBuffer, m_vertexBufferMemory, modelGpuVertices, BUFFER_ADDITIONAL_USAGE_VERTEX_BUFFER);
  //initalizeBuffer(m_indexBuffer, m_indexBufferMemory, modelGpuIndices, BUFFER_ADDITIONAL_USAGE_VERTEX_BUFFER);
}

// !TODO
void Model_Buffer::initalizeBuffer(core::Shared_Ptr<Buffer>         outBuffer,
                                   core::Shared_Ptr<Device_Memory>  outBufferMemory, 
                                   const core::span<const void>&    inData,
                                   Buffer_Additional_Usage          inAdditionalUsage)
{
  const VkDeviceSize bufferSize = inData.size();
  core::Shared_Ptr<const Device> device = m_commandPool->getDevice();

  auto stagingBuffer = core::makeShared<Buffer>(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  auto staggingBufferMemory = core::makeShared<Device_Memory>(device, stagingBuffer->getMemoryRequirements(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  
  staggingBufferMemory->bind(*stagingBuffer);
  core::Optional<void*> mappedData = staggingBufferMemory->map();
  if (mappedData.hasValue())
  {
    std::memcpy(mappedData.value(), inData.data(), bufferSize);
    staggingBufferMemory->unmap();
  }

  outBuffer = core::makeShared<Buffer>(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | inAdditionalUsage);
  outBufferMemory = core::makeShared<Device_Memory>(device, outBuffer->getMemoryRequirements(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  outBufferMemory->bind(*outBuffer);

  auto commandBuffer = core::makeShared<Command_Buffer>(m_commandPool->getDevice(), m_commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  commandBuffer->begin();
  commandBuffer->copyBuffer(stagingBuffer, outBuffer, bufferSize);
  commandBuffer->end();
  m_queue->sumbit(core::span(&commandBuffer, 1u));
  m_queue->waitIdle();
}

}
