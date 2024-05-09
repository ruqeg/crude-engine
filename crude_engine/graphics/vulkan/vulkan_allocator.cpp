#include <vulkan/vulkan_core.h>

module crude_engine.graphics.vulkan.vulkan_allocator;

import crude_engine.core.memory_system;
import crude_engine.core.assert;

namespace crude_engine
{

void* Vulkan_Allocator::allocation(
  size_t                                      size,
  size_t                                      alignment,
  VkSystemAllocationScope                     allocationScope)
{
  return Memory_System::Default_Aligned_Allocator::allocate(size, alignment);
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
  return Memory_System::Default_Aligned_Allocator::reallocate(pOriginal, size, alignment);
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
  assert(pMemory);

  Memory_System::Default_Aligned_Allocator::deallocate(pMemory);
}

void VKAPI_CALL Vulkan_Allocator::free(
  void* pUserData,
  void* pMemory)
{
  assert(pMemory);

  return static_cast<Vulkan_Allocator*>(pUserData)->free(pMemory);
}

}