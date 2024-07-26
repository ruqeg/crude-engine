module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.descriptor_set;

import crude.core.std_containers_heap;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;
class Descriptor_Set_Layout;
class Descriptor_Pool;

class Descriptor_Set : public Vulkan_Object<VkDescriptorSet>
{
public:
  explicit Descriptor_Set(core::shared_ptr<Descriptor_Pool> pool);
  explicit Descriptor_Set(core::shared_ptr<Descriptor_Pool> pool, const core::vector<core::shared_ptr<Descriptor_Set_Layout>>& setLayouts);
  ~Descriptor_Set();
public:
  core::shared_ptr<const Device> getDevice() noexcept;
  const core::vector<core::shared_ptr<Descriptor_Set_Layout>>& getSetLayouts() noexcept;
  core::shared_ptr<Descriptor_Pool> getPool() noexcept;
private:
  core::vector<core::shared_ptr<Descriptor_Set_Layout>>  m_setLayouts;
  core::shared_ptr<Descriptor_Pool>                      m_pool;
};

}
