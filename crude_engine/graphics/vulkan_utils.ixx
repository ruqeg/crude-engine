module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan_utils;

export namespace crude::graphics
{

void vulkanHandleResult(VkResult vkRes, const char* msg) { /*TODO*/ }
void vulkanHandleError(const char* msg) { /*TODO*/ }

}