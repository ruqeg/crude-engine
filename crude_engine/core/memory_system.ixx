module;

#include <malloc.h>

export module crude_engine.memory_system;

import crude_engine.memory_manager;

export namespace crude_engine
{

class Memory_System
{
public:
  class Default_Free_RBT_Allocator
  {
  public:
    template<class T, typename... Args>
    static [[nodiscard]] T* cxxAllocate(Args&&... args) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().cxxAllocate<T>(forward<Args>(args)...);
    }

    template<class T, typename... Args>
    static [[nodiscard]] T* cxxAllocate(size_t n, Args&&... args) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().cxxAllocate<T>(n, forward<Args>(args)...);
    }

    template<class T>
    static void cxxDeallocate(T* ptr) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().cxxDeallocate<T>(ptr);
    }

    template<class T>
    static void cxxDeallocate(size_t n, T* ptr) noexcept
    {
      return Memory_Manager::getInstance().getDefaultFreeRBTAllocators().cxxDeallocate<T>(n, ptr);
    }

    static [[nodiscard]] void* allocate(size_t size) noexcept
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
    static [[nodiscard]] T* cxxAllocate(Args&&... args) noexcept
    {
      return Memory_Manager::getInstance().getDefaultSystemAllocators().cxxAllocate<T>(forward<Args>(args)...);
    }

    template<class T, typename... Args>
    static [[nodiscard]] T* cxxAllocate(size_t n, Args&&... args) noexcept
    {
      return Memory_Manager::getInstance().getDefaultSystemAllocators().cxxAllocate<T>(n, forward<Args>(args)...);
    }

    template<class T>
    static void cxxDeallocate(T* ptr) noexcept
    {
      return Memory_Manager::getInstance().getDefaultSystemAllocators().cxxDeallocate<T>(ptr);
    }

    template<class T>
    static void cxxDeallocate(size_t n, T* ptr) noexcept
    {
      return Memory_Manager::getInstance().getDefaultSystemAllocators().cxxDeallocate<T>(n, ptr);
    }

    static [[nodiscard]] void* allocate(size_t size) noexcept
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

  using Default_Allocator = Default_Free_RBT_Allocator;
};

}  // namespace crude_engine