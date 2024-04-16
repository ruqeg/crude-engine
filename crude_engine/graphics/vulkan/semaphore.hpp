#pragma once

#include <core/shared_ptr.hpp>
#include <graphics/vulkan/object.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Device;

class Semaphore : public TObject<VkSemaphore>
{
public:
  explicit Semaphore(Shared_Ptr<const Device> device, VkSemaphoreCreateFlags flags = 0u);
  ~Semaphore();
private:
  Shared_Ptr<const Device>  m_device;
};

}
