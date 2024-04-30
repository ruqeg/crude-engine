#pragma once

#include <core/memory_manager.hpp>
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
    static CRUDE_NODISCARD T* cxxAllocate(Args&&... args) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().cxxAllocate<T>(Utility::forward<Args>(args)...);
    }

    template<class T, typename... Args>
    static CRUDE_NODISCARD T* cxxAllocate(std::size_t n, Args&&... args) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().cxxAllocate<T>(n, Utility::forward<Args>(args)...);
    }

    template<class T>
    static void cxxDeallocate(T* ptr) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().cxxDeallocate<T>(ptr);
    }

    template<class T>
    static void cxxDeallocate(std::size_t n, T* ptr) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().cxxDeallocate<T>(n, ptr);
    }

    static CRUDE_NODISCARD void* allocate(std::size_t size) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().allocate(size);
    }

    static void deallocate(void* ptr) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().deallocate(ptr);
    }

    static void reset() noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().reset();
    }
  };

  class Default_System_Allocator
  {
  public:
    template<class T, typename... Args>
    static CRUDE_NODISCARD T* cxxAllocate(Args&&... args) noexcept
    {
      return Memory_Manager::getInstance().getDefaultSystemAllocators().cxxAllocate<T>(Utility::forward<Args>(args)...);
    }

    template<class T, typename... Args>
    static CRUDE_NODISCARD T* cxxAllocate(std::size_t n, Args&&... args) noexcept
    {
      return Memory_Manager::getInstance().getDefaultSystemAllocators().cxxAllocate<T>(n, Utility::forward<Args>(args)...);
    }

    template<class T>
    static void cxxDeallocate(T* ptr) noexcept
    {
      return Memory_Manager::getInstance().getDefaultSystemAllocators().cxxDeallocate<T>(ptr);
    }

    template<class T>
    static void cxxDeallocate(std::size_t n, T* ptr) noexcept
    {
      return Memory_Manager::getInstance().getDefaultSystemAllocators().cxxDeallocate<T>(n, ptr);
    }

    static CRUDE_NODISCARD void* allocate(std::size_t size) noexcept
    {
      return Memory_Manager::getInstance().getDefaultSystemAllocators().allocate(size);
    }

    static void deallocate(void* ptr) noexcept
    {
      return Memory_Manager::getInstance().getDefaultSystemAllocators().deallocate(ptr);
    }

    static void reset() noexcept
    {
    }
  };

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
    static void deallocate(void* ptr)
    {
      _aligned_free(ptr);
    }
  };

  using Default_Allocator = Default_Free_RBT_Allocator;
};

}  // namespace crude_engine

#include <core/memory_system.inl>