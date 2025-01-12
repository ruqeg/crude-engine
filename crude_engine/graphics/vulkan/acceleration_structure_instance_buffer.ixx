module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.acceleration_structure_instance_buffer;

export import crude.gfx.vk.acceleration_structure_input_buffer;

import crude.core.utility;
import crude.gfx.vk.buffer;
import crude.gfx.vk.staging_buffer;
import crude.gfx.vk.device_memory;

export namespace crude::gfx::vk
{

template<class T>
class Acceleration_Structure_Instance_Buffer : public Acceleration_Structure_Input_Buffer
{
public:
  explicit Acceleration_Structure_Instance_Buffer(std::shared_ptr<Device> device, core::uint32 instanceCount) noexcept;
  const T& getInstance(core::uint32 index) const;
  T& getInstance(core::uint32 index);
  core::uint32 getInstanceCount() const { return m_instanceCount; }
private:
  core::uint32 m_instanceCount;
  T* m_instances;
};

}

namespace crude::gfx::vk
{

template<class T>
Acceleration_Structure_Instance_Buffer<T>::Acceleration_Structure_Instance_Buffer(std::shared_ptr<Device> device, core::uint32 instanceCount) noexcept
  : Acceleration_Structure_Input_Buffer(device, instanceCount * sizeof(T))
  , m_instanceCount{ instanceCount }
{
  core::shared_ptr<Staging_Buffer> stagingBuffer = core::allocateShared<Staging_Buffer>(device, m_size);
  m_instances = reinterpret_cast<T*>(stagingBuffer->getMemory()->mapUnsafe());
  if (m_instances)
  {
    for (core::uint32 i = 0; i < instanceCount; ++i)
    {
      core::constructAt(&m_instances[i], T());
    }
  }
}

template<class T>
T& Acceleration_Structure_Instance_Buffer<T>::getInstance(core::uint32 index)
{
  return m_instances[index];
}

template<class T>
const T& Acceleration_Structure_Instance_Buffer<T>::getInstance(core::uint32 index) const
{
  return m_instances[index];
}

}