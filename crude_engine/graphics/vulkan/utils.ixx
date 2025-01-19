module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.utils;

export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;
import crude.gfx.vk.vulkan_object;

export namespace crude::gfx::vk
{
class Buffer;
}

export namespace crude::gfx::vk
{

VkDeviceOrHostAddressConstKHR getAddress(core::shared_ptr<const Buffer> buffer);
template<class T>
T getObjectHandle(core::shared_ptr<Vulkan_Object<T>> object);
template<class T>
T getOptionalObjectHandle(const core::optional<core::shared_ptr<Vulkan_Object<T>>>& object);

}

namespace crude::gfx::vk
{

template<class T>
T getObjectHandle(core::shared_ptr<Vulkan_Object<T>> object)
{
  return object ? object->getHandle() : VK_NULL_HANDLE;
}

template<class T>
T getOptionalObjectHandle(const core::optional<core::shared_ptr<Vulkan_Object<T>>>& object)
{
  if (!object.has_value())
    return VK_NULL_HANDLE;
  return getObjectHandle<T>(object.value());
}

}