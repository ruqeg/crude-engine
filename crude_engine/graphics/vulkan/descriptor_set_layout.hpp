#pragma once

#include <graphics/vulkan/object.hpp>
#include <graphics/vulkan/include_vulkan.hpp>
#include <core/data_structures/shared_ptr.hpp>

namespace crude_engine
{

class Device;

class Descriptor_Set_Layout : public TObject<VkDescriptorSetLayout>
{
public:
  explicit Descriptor_Set_Layout(Shared_Ptr<const Device>       device,
                                 VkDescriptorSetLayoutBinding*  pBindings,
                                 uint32                         bindingCount);
  ~Descriptor_Set_Layout();
private:
  Shared_Ptr<const Device>  m_device;
};

}
