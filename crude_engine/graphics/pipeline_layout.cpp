#include <vulkan/vulkan.hpp>

module crude.graphics.pipeline_layout;

import crude.graphics.device;
import crude.graphics.descriptor_set_layout;
import crude.graphics.vulkan_utils;
import crude.core.std_containers_heap;
import crude.core.algorithms;

namespace crude::graphics
{

Pipeline_Layout::Pipeline_Layout(core::Shared_Ptr<const Device>                              device,
                                 const core::span<core::Shared_Ptr<Descriptor_Set_Layout>>&  descriptorSetLayouts,
                                 const core::span<VkPushConstantRange>&                      pushConstantRanges)
  :
  m_device(device)
{
  core::vector<VkDescriptorSetLayout> vkDescriptorSetLayoutHandles(descriptorSetLayouts.size());
  core::Algorithms::copyc(descriptorSetLayouts.begin(), descriptorSetLayouts.end(), vkDescriptorSetLayoutHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
  });

  VkPipelineLayoutCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  vkCreateInfo.flags                   = 0u;
  vkCreateInfo.pNext                   = nullptr;

  vkCreateInfo.setLayoutCount          = vkDescriptorSetLayoutHandles.size();
  vkCreateInfo.pSetLayouts             = vkDescriptorSetLayoutHandles.data();
  vkCreateInfo.pushConstantRangeCount  = static_cast<core::uint32>(pushConstantRanges.size());
  vkCreateInfo.pPushConstantRanges     = pushConstantRanges.data();

  VkResult result = vkCreatePipelineLayout(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create pipeline layout");
}

Pipeline_Layout::~Pipeline_Layout()
{
  vkDestroyPipelineLayout(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}