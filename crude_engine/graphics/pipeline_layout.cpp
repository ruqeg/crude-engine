#include <vulkan/vulkan.hpp>

module crude.graphics.pipeline_layout;

import crude.graphics.device;
import crude.graphics.descriptor_set_layout;
import crude.graphics.vulkan_utils;
import crude.core.std_containers_heap;
import crude.core.algorithms;

namespace crude::graphics
{

Pipeline_Layout::Pipeline_Layout(core::shared_ptr<const Device> device, core::shared_ptr<const Descriptor_Set_Layout> descriptorSetLayout)
  : Pipeline_Layout(device, core::vector{ descriptorSetLayout })
{}

Pipeline_Layout::Pipeline_Layout(core::shared_ptr<const Device>                 device,
                                 core::shared_ptr<const Descriptor_Set_Layout>  descriptorSetLayout, 
                                 const Push_Constant_Range_Base&                pushConstantRange)
  : Pipeline_Layout(device, core::vector { descriptorSetLayout }, core::vector{ pushConstantRange })
{}

Pipeline_Layout::Pipeline_Layout(core::shared_ptr<const Device>                                      device,
                                 const core::vector<core::shared_ptr<const Descriptor_Set_Layout>>&  descriptorSetLayouts,
                                 const core::vector<Push_Constant_Range_Base>&                       pushConstantRanges)
  : m_device(device)
  , m_setLayouts(descriptorSetLayouts)
  , m_pushConstantRanges(pushConstantRanges)
{
  core::vector<VkDescriptorSetLayout> vkDescriptorSetLayoutHandles(m_setLayouts.size());
  core::copyc(m_setLayouts.begin(), m_setLayouts.end(), vkDescriptorSetLayoutHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
  });

  VkPipelineLayoutCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  vkCreateInfo.flags                   = 0u;
  vkCreateInfo.pNext                   = nullptr;

  vkCreateInfo.setLayoutCount          = vkDescriptorSetLayoutHandles.size();
  vkCreateInfo.pSetLayouts             = vkDescriptorSetLayoutHandles.data();
  vkCreateInfo.pushConstantRangeCount  = static_cast<core::uint32>(m_pushConstantRanges.size());
  vkCreateInfo.pPushConstantRanges     = m_pushConstantRanges.data();

  VkResult result = vkCreatePipelineLayout(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create pipeline layout");
}

Pipeline_Layout::~Pipeline_Layout()
{
  vkDestroyPipelineLayout(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}