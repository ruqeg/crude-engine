#pragma once

#include <graphics/vulkan/include_vulkan.hpp>
#include <graphics/vulkan/object.hpp>
#include <core/data_structures/array_dynamic.hpp>
#include <core/data_structures/shared_ptr.hpp>

namespace crude_engine
{

class Device;
class Descriptor_Set_Layout;
class Descriptor_Pool;

class Descriptor_Set : public TObject<VkDescriptorSet>
{
public:
  explicit Descriptor_Set(Shared_Ptr<const Device>                                 device,
                          const Array_Dynamic<Shared_Ptr<Descriptor_Set_Layout>>&  setLayouts,
                          Shared_Ptr<Descriptor_Pool>                              pool);
  ~Descriptor_Set();
private:
  Shared_Ptr<const Device>                          m_device;
  Array_Dynamic<Shared_Ptr<Descriptor_Set_Layout>>  m_setLayouts;
  Shared_Ptr<Descriptor_Pool>                       m_pool;
};

}
