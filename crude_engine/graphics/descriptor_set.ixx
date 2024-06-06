module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.descriptor_set;

import crude.core.shared_ptr;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;
class Descriptor_Set_Layout;
class Descriptor_Pool;

class Descriptor_Set : public Vulkan_Object<VkDescriptorSet>
{
public:
  explicit Descriptor_Set(core::Shared_Ptr<const Device>            device,
                          core::Shared_Ptr<Descriptor_Pool>         pool,
                          core::Shared_Ptr<Descriptor_Set_Layout>&  setLayout);
  ~Descriptor_Set();
public:
  core::Shared_Ptr<const Device> getDevice() noexcept;
  core::Shared_Ptr<Descriptor_Set_Layout> getSetLayout() noexcept;
  core::Shared_Ptr<Descriptor_Pool> getPool() noexcept;
private:
  core::Shared_Ptr<const Device>           m_device;
  core::Shared_Ptr<Descriptor_Set_Layout>  m_setLayout;
  core::Shared_Ptr<Descriptor_Pool>        m_pool;
};

}
