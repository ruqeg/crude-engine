#include <vulkan/vulkan_core.h>

module crude.graphics.vulkan_allocator;

import crude.core.memory;
import crude.core.assert;

namespace crude::graphics
{

void* Vulkan_Allocator::allocation(
  size_t                                      size,
  size_t                                      alignment,
  VkSystemAllocationScope                     allocationScope)
{
  return core::getDefaultAlignedAllocator()->allocate(size, alignment);
}

void* VKAPI_CALL Vulkan_Allocator::allocation(
  void* pUserData,
  size_t                                      size,
  size_t                                      alignment,
  VkSystemAllocationScope                     allocationScope)
{
  return static_cast<Vulkan_Allocator*>(pUserData)->allocation(size,
    alignment,
    allocationScope);
}

void* Vulkan_Allocator::reallocation(
  void* pOriginal,
  size_t                                      size,
  size_t                                      alignment,
  VkSystemAllocationScope                     allocationScope)
{
  return core::getDefaultAlignedAllocator()->reallocate(pOriginal, size, alignment);
}

void* VKAPI_CALL Vulkan_Allocator::reallocation(
  void* pUserData,
  void* pOriginal,
  size_t                                      size,
  size_t                                      alignment,
  VkSystemAllocationScope                     allocationScope)
{
  return static_cast<Vulkan_Allocator*>(pUserData)->reallocation(pOriginal,
    size,
    alignment,
    allocationScope);
}

void Vulkan_Allocator::free(
  void* pMemory)
{
  core::assert(pMemory);
  core::getDefaultAlignedAllocator()->deallocate(pMemory);
}

void VKAPI_CALL Vulkan_Allocator::free(
  void* pUserData,
  void* pMemory)
{
  core::assert(pMemory);

  return static_cast<Vulkan_Allocator*>(pUserData)->free(pMemory);
}

}