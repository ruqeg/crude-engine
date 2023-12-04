#include "pipeline_layout.hpp"
#include "device.hpp"
#include "descriptor_set_layout.hpp"

namespace crude_engine
{

Pipeline_Layout_Create_Info::Pipeline_Layout_Create_Info(std::shared_ptr<const Device>                                     device,
                                                         const std::vector<std::shared_ptr<const Descriptor_Set_Layout>>&  descriptorSetLayouts,
                                                         const std::vector<VkPushConstantRange>&                           pushConstantRanges)
  :
  device(device),
  descriptorSetLayouts(descriptorSetLayouts),
  pushConstantRanges(pushConstantRanges)
{}

Pipeline_Layout::Pipeline_Layout(const Pipeline_Layout_Create_Info& createInfo)
{
  m_device = createInfo.device;

  const uint32 descriptorSetLayoutsCount = createInfo.descriptorSetLayouts.size();

  std::vector<VkDescriptorSetLayout> vkDescriptorSetLayoutHandles(descriptorSetLayoutsCount);
  for (uint32 i = 0; i < descriptorSetLayoutsCount; ++i)
  {
    vkDescriptorSetLayoutHandles[i] = createInfo.descriptorSetLayouts[i]->getHandle();
  }

  VkPipelineLayoutCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  vkCreateInfo.flags                   = 0u;
  vkCreateInfo.pNext                   = nullptr;

  vkCreateInfo.setLayoutCount          = vkDescriptorSetLayoutHandles.size();
  vkCreateInfo.pSetLayouts             = vkDescriptorSetLayoutHandles.data();
  vkCreateInfo.pushConstantRangeCount  = static_cast<uint32>(createInfo.pushConstantRanges.size());
  vkCreateInfo.pPushConstantRanges     = createInfo.pushConstantRanges.data();

  VkResult result = vkCreatePipelineLayout(CRUDE_VULKAN_01_HANDLE(m_device), &vkCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create pipeline layout");
}

Pipeline_Layout::~Pipeline_Layout()
{
  vkDestroyPipelineLayout(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}

}