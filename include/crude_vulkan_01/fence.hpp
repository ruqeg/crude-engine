#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"

namespace crude_vulkan_01
{

class Device;

struct Fence_Create_Info
{
  std::shared_ptr<const Device>  device;
  VkFenceCreateFlags             flags;
  explicit Fence_Create_Info(std::shared_ptr<const Device> device, VkFenceCreateFlags flags);
};

class Fence : public TObject<VkFence>
{
public:
  explicit Fence(const Fence_Create_Info& createInfo);
  ~Fence();
private:
  std::shared_ptr<const Device>  m_device;
};

}
