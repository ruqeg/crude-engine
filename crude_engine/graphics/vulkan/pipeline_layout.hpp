#pragma once

#include <core/data_structures/shared_ptr.hpp>
#include <core/data_structures/array_unsafe.hpp>
#include <graphics/vulkan/include_vulkan.hpp>
#include <graphics/vulkan/object.hpp>

namespace crude_engine
{

class Device;
class Descriptor_Set_Layout;

class Pipeline_Layout : public TObject<VkPipelineLayout>
{
public:
  explicit Pipeline_Layout(Shared_Ptr<const Device>                                      device,
                           const Array_Unsafe<Shared_Ptr<const Descriptor_Set_Layout>>&  descriptorSetLayouts,
                           const Array_Unsafe<VkPushConstantRange>&                      pushConstantRanges);
  ~Pipeline_Layout();
private:
  Shared_Ptr<const Device>  m_device;
};

}