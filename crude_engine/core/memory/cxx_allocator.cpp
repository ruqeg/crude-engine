#include <core/memory/cxx_allocator.hpp>
#include <core/memory/memory_utils.hpp>
#include <core/debug/assert.hpp>

namespace crude_engine
{

void* CXX_Allocator::allocate(std::size_t size) noexcept
{
  void* ptr = Memory_Utils::allocate(size);
  return ptr;
}

void CXX_Allocator::free(void* ptr) noexcept
{
  CRUDE_ASSERT(ptr);
  Memory_Utils::free(ptr);
}

}