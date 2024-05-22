module crude.core.system_allocator;

import crude.core.assert;

namespace crude::core
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