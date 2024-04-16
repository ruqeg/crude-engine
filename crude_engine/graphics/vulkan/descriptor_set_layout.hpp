#pragma once

#include <core/shared_ptr.hpp>
#include <core/array_unsafe.hpp>
#include <graphics/vulkan/object.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Device;

class Descriptor_Set_Layout : public TObject<VkDescriptorSetLayout>
{
public:
  explicit Descriptor_Set_Layout(Shared_Ptr<const Device>                           device,
                                 const Array_Unsafe<VkDescriptorSetLayoutBinding>&  bindings);
  ~Descriptor_Set_Layout();
private:
  Shared_Ptr<const Device>  m_device;
};

}
