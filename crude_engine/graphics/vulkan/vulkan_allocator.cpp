#include <graphics/vulkan/vulkan_allocator.hpp>
#include <core/memory/memory_system.hpp>
#include <core/debug/assert.hpp>

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
  CRUDE_ASSERT(ptr);

  Memory_System::Default_Aligned_Allocator::free(pMemory);
}

void VKAPI_CALL Vulkan_Allocator::free(
  void* pUserData,
  void* pMemory)
{
  CRUDE_ASSERT(ptr);

  return static_cast<Vulkan_Allocator*>(pUserData)->free(pMemory);
}

}