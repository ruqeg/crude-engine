module;

#include <vulkan/vulkan.h>
#include <utility>
#include <stdexcept>

export module crude.gfx.vk.extension;

export import crude.core.std_containers_heap;
import crude.core.logger;
import crude.core.assert;

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
constexpr const char* pfnToProcAddr() noexcept 
{
  // !TODO idk let it be for now?
  if constexpr (std::same_as<Fn, PFN_vkCreateDebugUtilsMessengerEXT>)
  {
    return "vkCreateDebugUtilsMessengerEXT";
  }
  if constexpr (std::same_as<Fn, PFN_vkDestroyDebugUtilsMessengerEXT>)
  {
    return "vkDestroyDebugUtilsMessengerEXT";
  }
  if constexpr (std::same_as<Fn, PFN_vkCmdDrawMeshTasksEXT>)
  {
    return "vkCmdDrawMeshTasksEXT";
  }
  if constexpr (std::same_as<Fn, PFN_vkCmdPushDescriptorSetKHR>)
  {
    return "vkCmdPushDescriptorSetKHR";
  }
  if constexpr (std::same_as<Fn, PFN_vkGetBufferDeviceAddressKHR>)
  {
    return "vkGetBufferDeviceAddressKHR";
  }
  if constexpr (std::same_as<Fn, PFN_vkGetAccelerationStructureBuildSizesKHR>)
  {
    return "vkGetAccelerationStructureBuildSizesKHR";
  }
  if constexpr (std::same_as<Fn, PFN_vkCreateAccelerationStructureKHR>)
  {
    return "vkCreateAccelerationStructureKHR";
  }
  if constexpr (std::same_as<Fn, PFN_vkGetAccelerationStructureDeviceAddressKHR>)
  {
    return "vkGetAccelerationStructureDeviceAddressKHR";
  }
  if constexpr (std::same_as<Fn, PFN_vkCmdBuildAccelerationStructuresKHR>)
  {
    return "vkCmdBuildAccelerationStructuresKHR";
  }
  if constexpr (std::same_as<Fn, PFN_vkCreateRayTracingPipelinesKHR>)
  {
    return "PFN_vkCreateRayTracingPipelinesKHR";
  }
  
  core::logError(core::Debug::Channel::Graphics, "Failed to find procAddr for %s!", typeid(Fn).name());
  core::assert(false);
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