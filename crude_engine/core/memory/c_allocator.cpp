#include <core/memory/c_allocator.hpp>
#include <core/memory/memory_utils.hpp>

namespace crude_engine
{

void* C_Allocator::allocate(std::size_t size) noexcept
{
  void* ptr = Memory_Utils::allocate(size);
  return ptr;
}

void C_Allocator::free(void* ptr) noexcept
{
  free(ptr);
}

}