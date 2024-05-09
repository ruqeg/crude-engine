#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.pipeline_layout;

import crude_engine.graphics.vulkan.device;
import crude_engine.graphics.vulkan.descriptor_set_layout;
import crude_engine.graphics.vulkan.vulkan_utils;
import crude_engine.core.std_containers_heap;
import crude_engine.core.algorithms;

namespace crude_engine
{

Pipeline_Layout::Pipeline_Layout(Shared_Ptr<const Device>                        device,
                                 const span<Shared_Ptr<Descriptor_Set_Layout>>&  descriptorSetLayouts,
                                 const span<VkPushConstantRange>&                pushConstantRanges)
  :
  m_device(device)
{
  vector<VkDescriptorSetLayout> vkDescriptorSetLayoutHandles(descriptorSetLayouts.size());
  Algorithms::copyc(descriptorSetLayouts.begin(), descriptorSetLayouts.end(), vkDescriptorSetLayoutHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
  });

  VkPipelineLayoutCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  vkCreateInfo.flags                   = 0u;
  vkCreateInfo.pNext                   = nullptr;

  vkCreateInfo.setLayoutCount          = vkDescriptorSetLayoutHandles.size();
  vkCreateInfo.pSetLayouts             = vkDescriptorSetLayoutHandles.data();
  vkCreateInfo.pushConstantRangeCount  = static_cast<uint32>(pushConstantRanges.size());
  vkCreateInfo.pPushConstantRanges     = pushConstantRanges.data();

  VkResult result = vkCreatePipelineLayout(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create pipeline layout");
}

Pipeline_Layout::~Pipeline_Layout()
{
  vkDestroyPipelineLayout(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}