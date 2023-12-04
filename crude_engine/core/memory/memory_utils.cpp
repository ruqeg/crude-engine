#include <core/memory/memory_utils.hpp>
#include <cstdlib>

namespace crude_engine
{

void* Memory_Utils::allocate(std::size_t size) noexcept
{
  void* memoryPtr = std::malloc(size);
  return reinterpret_cast<void*>(memoryPtr);
}

void Memory_Utils::free(void* memoryPtr) noexcept
{
  std::free(memoryPtr);
}

}