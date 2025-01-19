#include <vulkan/vulkan.hpp>

module crude.gfx.vk.pipeline;

import crude.gfx.vk.device;
import crude.gfx.vk.pipeline_layout;

namespace crude::gfx::vk
{

crude::gfx::vk::Pipeline::Pipeline(VkPipelineBindPoint bindPoint, core::shared_ptr<Pipeline_Layout> pipelineLayout)
  : m_device{ pipelineLayout->getDevice() }
  , m_pipelineLayout{ pipelineLayout }
  , m_bindPoint{ bindPoint }
{}

Pipeline::~Pipeline()
{
  vkDestroyPipeline(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

core::shared_ptr<Pipeline_Layout> Pipeline::getPipelineLayout() const 
{ 
  return m_pipelineLayout; 
}

core::shared_ptr<const Device> Pipeline::getDevice() const
{ 
  return m_device;
}

VkPipelineBindPoint Pipeline::getBindPoint() const
{ 
  return m_bindPoint; 
}

}