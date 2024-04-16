#include <graphics/vulkan/pipeline_layout.hpp>
#include <graphics/vulkan/device.hpp>
#include <graphics/vulkan/descriptor_set_layout.hpp>
#include <core/array_dynamic.hpp>

namespace crude_engine
{

Pipeline_Layout::Pipeline_Layout(Shared_Ptr<const Device>                                device,
                                 const Array_Unsafe<Shared_Ptr<Descriptor_Set_Layout>>&  descriptorSetLayouts,
                                 const Array_Unsafe<VkPushConstantRange>&                pushConstantRanges)
  :
  m_device(device)
{
  Array_Dynamic<VkDescriptorSetLayout> vkDescriptorSetLayoutHandles(descriptorSetLayouts.size());
  Algorithms::copyc(descriptorSetLayouts.begin(), descriptorSetLayouts.end(), vkDescriptorSetLayoutHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = CRUDE_OBJECT_HANDLE(*src);
  });

  VkPipelineLayoutCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  vkCreateInfo.flags                   = 0u;
  vkCreateInfo.pNext                   = nullptr;

  vkCreateInfo.setLayoutCount          = vkDescriptorSetLayoutHandles.size();
  vkCreateInfo.pSetLayouts             = vkDescriptorSetLayoutHandles.data();
  vkCreateInfo.pushConstantRangeCount  = static_cast<uint32>(pushConstantRanges.size());
  vkCreateInfo.pPushConstantRanges     = pushConstantRanges.data();

  VkResult result = vkCreatePipelineLayout(CRUDE_OBJECT_HANDLE(m_device), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create pipeline layout");
}

Pipeline_Layout::~Pipeline_Layout()
{
  vkDestroyPipelineLayout(CRUDE_OBJECT_HANDLE(m_device), m_handle, getPVkAllocationCallbacks());
}

}