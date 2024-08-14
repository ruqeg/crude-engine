module;

#include <vulkan/vulkan.h>

export module crude.graphics.uniform_buffer;

export import crude.core.std_containers_stack;
export import crude.graphics.buffer;

export namespace crude::graphics
{

class Device;

class Uniform_Buffer_Base : public Buffer
{
public:
  explicit Uniform_Buffer_Base(core::shared_ptr<const Device> device, VkDeviceSize size)
    : Buffer({
      .device = device, 
      .size = size,
      .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      .memoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT})
  {}
};

template<class T>
class Uniform_Buffer : public Uniform_Buffer_Base
{
public:
  explicit Uniform_Buffer(core::shared_ptr<const Device> device);
  core::optional<T*> map();
  T* mapUnsafe();
  void unmap();
};

template<class T>
Uniform_Buffer<T>::Uniform_Buffer(core::shared_ptr<const Device> device)
  :
  Uniform_Buffer_Base(device, sizeof(T))
{}

template<class T>
core::optional<T*> Uniform_Buffer<T>::map()
{
  core::optional<void*> data = m_memory->map();
  return data.hasValue() ? static_cast<T*>(data.value()) : core::nullopt;
}

template<class T>
T* Uniform_Buffer<T>::mapUnsafe()
{
  void* data = m_memory->mapUnsafe();
  return static_cast<T*>(data);
}

template<class T>
void Uniform_Buffer<T>::unmap()
{
  m_memory->unmap();
}

}
