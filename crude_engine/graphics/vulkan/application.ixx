module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.application;

import crude.core.alias;

export namespace crude::gfx::vk
{

struct Application final : public VkApplicationInfo
{
public:
  struct Initialize
  {
    const char*   pApplicationName   = "default application name";
    core::uint32  applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    const char*   pEngineName        = "default engine name";
    core::uint32  engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    core::uint32  apiVersion         = VK_API_VERSION_1_0;
  };
public:
  explicit Application(const Initialize& info)
  {
    this->sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    this->pNext              = nullptr;

    this->pApplicationName   = info.pApplicationName;
    this->applicationVersion = info.applicationVersion;
    this->pEngineName        = info.pEngineName;
    this->engineVersion      = info.engineVersion;
    this->apiVersion         = info.apiVersion;
  }
};

}
