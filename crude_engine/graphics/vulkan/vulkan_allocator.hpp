// get from Vulkan Programming Guide: The Official Guide to Learning Vulkan

#pragma once

#include <vulkan/vulkan_core.h>

namespace crude_engine
{

class Vulkan_Allocator
{
public:
  inline operator VkAllocationCallbacks() const
  {
    VkAllocationCallbacks result;

    result.pUserData             = (void*)this;
    result.pfnAllocation         = allocation;
    result.pfnReallocation       = reallocation;
    result.pfnFree               = free;
    result.pfnInternalAllocation = nullptr;
    result.pfnInternalFree       = nullptr;

    return result;
  };

private:
  static void* VKAPI_CALL allocation(
    void*                                       pUserData,
    size_t                                      size,
    size_t                                      alignment,
    VkSystemAllocationScope                     allocationScope);

  static void* VKAPI_CALL reallocation(
    void*                                       pUserData,
    void*                                       pOriginal,
    size_t                                      size,
    size_t                                      alignment,
    VkSystemAllocationScope                     allocationScope);

  static void VKAPI_CALL free(
    void*                                       pUserData,
    void*                                       pMemory);

  void* allocation(
    size_t                                      size,
    size_t                                      alignment,
    VkSystemAllocationScope                     allocationScope);

  void* reallocation(
    void* pOriginal,
    size_t                                      size,
    size_t                                      alignment,
    VkSystemAllocationScope                     allocationScope);

  void free(
    void*                                       pMemory);
};

}
