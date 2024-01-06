#pragma once

#include <core/memory/memory_manager.hpp>
#include <malloc.h>

namespace crude_engine
{

class Memory_System
{
public:
  class Default_Free_RBT_Allocator
  {
  public:
    template<class T, typename... Args>
    static CRUDE_NODISCARD T* mnew(Args&&... args) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().mnew<T>(Utility::forward<Args>(args)...);
    }

    template<class T, typename... Args>
    static CRUDE_NODISCARD T* mnewArray(std::size_t n, Args&&... args) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().mnewArray<T>(n, Utility::forward<Args>(args)...);
    }

    template<class T>
    static void mdelete(T* ptr) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().mdelete<T>(ptr);
    }

    template<class T>
    static void mdeleteArray(std::size_t n, T* ptr) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().mdeleteArray<T>(n, ptr);
    }

    static CRUDE_NODISCARD void* allocate(std::size_t size) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().allocate(size);
    }

    static void free(void* ptr) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().free(ptr);
    }

    static void reset() noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().reset();
    }
  };

  // !TODO
  class Default_Aligned_Allocator
  {
  public:
    static CRUDE_NODISCARD void* allocate(std::size_t size, std::size_t alignment)
    {
      return _aligned_malloc(size, alignment);
    }
    static CRUDE_NODISCARD void* reallocate(void* ptr, std::size_t size, std::size_t alignment)
    {
      return _aligned_realloc(ptr, size, alignment);
    }
    static void free(void* ptr)
    {
      _aligned_free(ptr);
    }
  };

  using Default_Allocator = Default_Free_RBT_Allocator;
};

}  // namespace crude_engine

#include <core/memory/memory_system.inl>