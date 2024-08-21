module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan_utils;

import crude.core.logger;

export namespace crude::graphics
{

void vulkanHandleResult(VkResult vkres, const char* msg)
{
  if (vkres == VK_SUCCESS)
    return;
  core::logError(core::Debug::Channel::Graphics, "[VkResult %d] %s", vkres, msg);
}

void vulkanHandleError(const char* msg)
{ 
  core::logError(core::Debug::Channel::Graphics, "%s", msg);
}

}