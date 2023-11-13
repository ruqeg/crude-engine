#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"

namespace crude_vulkan_01
{

class Device;

struct Command_Pool_Create_Info
{
  std::shared_ptr<const Device>  device;
  VkCommandPoolCreateFlags       flags;
  uint32                         queueFamilyIndex;
  explicit Command_Pool_Create_Info(std::shared_ptr<const Device>  device,
                                    VkCommandPoolCreateFlags       flags,
                                    uint32                         queueFamilyIndex);
};

class Command_Pool : public TObject<VkCommandPool>
{
public:
  explicit Command_Pool(const Command_Pool_Create_Info& createInfo);
  ~Command_Pool();
private:
  std::shared_ptr<const Device>  m_device;
};

}
