#include <cstdlib>

module crude_engine.memory_utils;

namespace crude_engine
{

void* Memory_Utils::allocate(size_t size) noexcept
{
  void* memoryPtr = std::malloc(size);
  return reinterpret_cast<void*>(memoryPtr);
}

void Memory_Utils::free(void* memoryPtr) noexcept
{
  std::free(memoryPtr);
}

}