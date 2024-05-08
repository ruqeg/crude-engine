module crude_engine.core.system_allocator;

import crude_engine.core.assert;

namespace crude_engine
{

void* System_Allocator::allocate(size_t size) noexcept
{
  void* ptr = Memory_Utils::allocate(size);
  return ptr;
}

void System_Allocator::deallocate(void* ptr) noexcept
{
  assert(ptr);
  Memory_Utils::free(ptr);
}

}