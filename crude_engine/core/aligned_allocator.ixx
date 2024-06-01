module;

#include <malloc.h>

export module crude.core.aligned_allocator;

export namespace crude::core
{

class Aligned_Allocator
{
public:
  static [[nodiscard]] void* allocate(size_t size, size_t alignment)
  {
    return _aligned_malloc(size, alignment);
  }
  static [[nodiscard]] void* reallocate(void* ptr, size_t size, size_t alignment)
  {
    return _aligned_realloc(ptr, size, alignment);
  }
  static void deallocate(void* ptr)
  {
    _aligned_free(ptr);
  }
};

}