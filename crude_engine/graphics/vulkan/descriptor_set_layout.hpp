#pragma once

#include <core/std_containers.hpp>
#include <core/shared_ptr.hpp>
#include <graphics/vulkan/object.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Device;

class Descriptor_Set_Layout : public TObject<VkDescriptorSetLayout>
{
public:
  explicit Descriptor_Set_Layout(Shared_Ptr<const Device>                   device,
                                 const span<VkDescriptorSetLayoutBinding>&  bindings);
  ~Descriptor_Set_Layout();
private:
  Shared_Ptr<const Device>  m_device;
};

}
