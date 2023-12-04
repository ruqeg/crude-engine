#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"

namespace crude_engine
{

class Device;

struct Semaphore_Create_Info
{
  std::shared_ptr<const Device>  device;
  VkSemaphoreCreateFlags         flags;
  explicit Semaphore_Create_Info(std::shared_ptr<const Device> device, VkSemaphoreCreateFlags flags = 0u);
};

class Semaphore : public TObject<VkSemaphore>
{
public:
  explicit Semaphore(const Semaphore_Create_Info& createInfo);
  ~Semaphore();
private:
  std::shared_ptr<const Device>  m_device;
};

}
