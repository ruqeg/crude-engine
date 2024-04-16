#pragma once

#include <core/shared_ptr.hpp>
#include <graphics/vulkan/object.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Device;

class Command_Pool : public TObject<VkCommandPool>
{
public:
  explicit Command_Pool(Shared_Ptr<const Device>  device,
                        VkCommandPoolCreateFlags  flags,
                        uint32                    queueFamilyIndex);
  ~Command_Pool();
private:
  Shared_Ptr<const Device>  m_device;
};

}
