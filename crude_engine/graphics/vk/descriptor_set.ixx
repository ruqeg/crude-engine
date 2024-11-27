module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.descriptor_set;

import crude.core.std_containers_heap;
import crude.gfx.vk.vulkan_object;

export namespace crude::gfx::vk
{

class Device;
class Descriptor_Set_Layout;
class Descriptor_Pool;

class Descriptor_Set : public Vulkan_Object<VkDescriptorSet>
{
public:
  explicit Descriptor_Set(core::shared_ptr<Descriptor_Pool> pool, core::shared_ptr<Descriptor_Set_Layout> setLayout);
  ~Descriptor_Set();
public:
  core::shared_ptr<const Device> getDevice() noexcept;
  core::shared_ptr<Descriptor_Set_Layout> getSetLayout() noexcept;
  core::shared_ptr<Descriptor_Pool> getPool() noexcept;
private:
  core::shared_ptr<Descriptor_Set_Layout>  m_setLayout;
  core::shared_ptr<Descriptor_Pool>        m_pool;
};

}
