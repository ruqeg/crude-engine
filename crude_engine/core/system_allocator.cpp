#include <core/system_allocator.hpp>
#include <core/memory_utils.hpp>
#include <core/assert.hpp>

namespace crude_engine
{

void* System_Allocator::allocate(std::size_t size) noexcept
{
  void* ptr = Memory_Utils::allocate(size);
  return ptr;
}

void System_Allocator::deallocate(void* ptr) noexcept
{
  CRUDE_ASSERT(ptr);
  Memory_Utils::free(ptr);
}

}