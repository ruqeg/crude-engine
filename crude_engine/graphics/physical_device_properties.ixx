module;

#include <vulkan/vulkan.h>

export module crude.graphics.physical_device_properties;

export namespace crude::graphics
{

class Physical_Device_Properties
{
public:
  Physical_Device_Properties() = default;
  explicit Physical_Device_Properties(VkPhysicalDeviceProperties properties) noexcept : m_properties(properties) {}
  const char* getDeviceName() const noexcept { return m_properties.deviceName; }
  VkPhysicalDeviceType getDeviceType() const noexcept { return m_properties.deviceType; }
public:
  VkSampleCountFlagBits getMaximumUsableSampleCount()
  {
    VkSampleCountFlags counts = m_properties.limits.framebufferColorSampleCounts & m_properties.limits.framebufferDepthSampleCounts;
    if (counts & VK_SAMPLE_COUNT_64_BIT)  return VK_SAMPLE_COUNT_64_BIT;
    if (counts & VK_SAMPLE_COUNT_32_BIT)  return VK_SAMPLE_COUNT_32_BIT;
    if (counts & VK_SAMPLE_COUNT_16_BIT)  return VK_SAMPLE_COUNT_16_BIT;
    if (counts & VK_SAMPLE_COUNT_8_BIT)   return VK_SAMPLE_COUNT_8_BIT;
    if (counts & VK_SAMPLE_COUNT_4_BIT)   return VK_SAMPLE_COUNT_4_BIT;
    if (counts & VK_SAMPLE_COUNT_2_BIT)   return VK_SAMPLE_COUNT_2_BIT;
    return VK_SAMPLE_COUNT_1_BIT;
  }
private:
  VkPhysicalDeviceProperties m_properties;
};

}