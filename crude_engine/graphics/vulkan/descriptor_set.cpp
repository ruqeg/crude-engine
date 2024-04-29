#include <graphics/vulkan/descriptor_set.hpp>
#include <graphics/vulkan/descriptor_pool.hpp>
#include <graphics/vulkan/descriptor_set_layout.hpp>
#include <graphics/vulkan/device.hpp>

namespace crude_engine
{

Descriptor_Set::Descriptor_Set(Shared_Ptr<const Device>                          device,
                               const vector<Shared_Ptr<Descriptor_Set_Layout>>&  setLayouts,
                               Shared_Ptr<Descriptor_Pool>                       pool)
  :
  m_device(device),
  m_pool(pool),
  m_setLayouts(setLayouts)
{
  vector<VkDescriptorSetLayout> setLayoutsHandles(m_setLayouts.size());
  Algorithms::copyc(m_setLayouts.begin(), m_setLayouts.end(), setLayoutsHandles.begin(), [](auto& s, auto& d) -> void {
    *d = CRUDE_OBJECT_HANDLE(*s);
  });

  VkDescriptorSetAllocateInfo vkAllocateInfo{};
  vkAllocateInfo.sType               = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  vkAllocateInfo.pNext               = nullptr;
  vkAllocateInfo.pSetLayouts         = setLayoutsHandles.data();
  vkAllocateInfo.descriptorSetCount  = static_cast<uint32>(setLayoutsHandles.size());
  vkAllocateInfo.descriptorPool      = CRUDE_OBJECT_HANDLE(m_pool);

  VkResult result = vkAllocateDescriptorSets(CRUDE_OBJECT_HANDLE(m_device), &vkAllocateInfo, &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to allocate descriptor set");
}

Descriptor_Set::~Descriptor_Set()
{
  if (m_pool->canFreeDescriptorSet())
    vkFreeDescriptorSets(CRUDE_OBJECT_HANDLE(m_device), CRUDE_OBJECT_HANDLE(m_pool), 1u, &m_handle);
}

}
