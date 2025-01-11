module;

#include <vulkan/vulkan.h>
#include <utility>

export module crude.gfx.vk.extension;

export import crude.core.std_containers_heap;
import crude.core.logger;

export namespace crude::gfx::vk
{

class Instance;
class Device;

}

namespace crude::gfx::vk
{

PFN_vkVoidFunction getInstanceProcAddr(core::shared_ptr<const Instance> instance, const char* procName);
PFN_vkVoidFunction gettDeviceProcAddr(core::shared_ptr<const Device> device, const char* procName);

template<class Fn>
constexpr const char* pfnToProcAddr()
{
  if constexpr (std::same_as<Fn, PFN_vkCreateDebugUtilsMessengerEXT>)
  {
    return "vkCreateDebugUtilsMessengerEXT";
  }
  else if constexpr (std::same_as<Fn, PFN_vkDestroyDebugUtilsMessengerEXT>)
  {
    return "vkDestroyDebugUtilsMessengerEXT";
  }
  else if constexpr (std::same_as<Fn, PFN_vkCmdDrawMeshTasksEXT>)
  {
    return "vkCmdDrawMeshTasksEXT";
  }
  else if constexpr (std::same_as<Fn, PFN_vkCmdPushDescriptorSetKHR>)
  {
    return "vkCmdPushDescriptorSetKHR";
  }
  else if constexpr (std::same_as<Fn, PFN_vkGetBufferDeviceAddressKHR>)
  {
    return "vkGetBufferDeviceAddressKHR";
  }

  core::logError(core::Debug::Channel::Graphics, "Failed to converty pfn to procAddr!");
  return "";
}

}

export namespace crude::gfx::vk
{

template<class Fn>
class Extension
{
public:
  operator Fn() const noexcept { return m_procAddr; }
  operator bool() const noexcept { return m_procAddr != nullptr; }
protected:
  Extension(PFN_vkVoidFunction procAddr) noexcept
    : m_procAddr(reinterpret_cast<Fn>(procAddr)) {}
private:
  const Fn m_procAddr;
};

template<class Fn>
class Instance_Extension : public Extension<Fn>
{
public:
  explicit Instance_Extension(core::shared_ptr<const Instance> instance) noexcept
    : Extension<Fn>(getInstanceProcAddr(instance, pfnToProcAddr<Fn>())) {}
};

template<class Fn>
class Device_Extension : public Extension<Fn>
{
public:
  explicit Device_Extension(core::shared_ptr<const Device> device) noexcept
    : Extension<Fn>(gettDeviceProcAddr(device, pfnToProcAddr<Fn>())) {}
};

template<class Fn>
const Instance_Extension<Fn>& getInstanceExtension(core::shared_ptr<const Instance> instance)
{
  static Instance_Extension<Fn> extension(instance);
  return extension;
}

template<class Fn>
const Device_Extension<Fn>& getDeviceExtension(core::shared_ptr<const Device> device)
{
  static Device_Extension<Fn> extension(device);
  return extension;
}

}