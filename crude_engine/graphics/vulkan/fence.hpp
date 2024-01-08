#pragma once

#include <core/data_structures/shared_ptr.hpp>
#include <graphics/vulkan/include_vulkan.hpp>
#include <graphics/vulkan/object.hpp>

namespace crude_engine
{

class Device;

class Fence : public TObject<VkFence>
{
public:
  explicit Fence(Shared_Ptr<const Device> device, VkFenceCreateFlags flags);
  ~Fence();
  bool wait(uint64 timeout = UINT64_MAX);
  bool reset();
private:
  Shared_Ptr<const Device>  m_device;
};

}
