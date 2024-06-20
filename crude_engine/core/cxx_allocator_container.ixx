module;

#include <utility>

export module crude.core.cxx_allocator_container;

import crude.core.iallocator;
import crude.core.utility;

export namespace crude::core
{

class CXX_Allocator_Container : public IAllocator
{
public:
  template<class T, typename... Args>
  [[nodiscard]] T* cxxAllocate(Args&&... args) noexcept;

  template<class T, typename... Args>
  [[nodiscard]] T* cxxAllocate(size_t n, Args&&... args) noexcept;

  template<class T>
  void cxxDeallocate(T* ptr) noexcept;

  template<class T>
  void cxxDeallocate(size_t n, T* ptr) noexcept;
};

template<class T, typename ...Args>
[[nodiscard]] T* CXX_Allocator_Container::cxxAllocate(Args&& ...args) noexcept
{
  T* ptr = static_cast<T*>(allocate(sizeof(T)));
  return constructAt(ptr, std::forward<Args>(args)...);
}

template<class T, typename ...Args>
[[nodiscard]] T* CXX_Allocator_Container::cxxAllocate(size_t n, Args && ...args) noexcept
{
  T* ptr = static_cast<T*>(allocate(sizeof(T) * n));

  for (size_t i = 0; i < n; ++i)
  {
    constructAt(ptr + i, std::forward<Args>(args)...);
  }
  return ptr;
}

template<class T>
void CXX_Allocator_Container::cxxDeallocate(T* ptr) noexcept
{
  destructorAt(ptr);
  deallocate((void*)(ptr));
}

template<class T>
void CXX_Allocator_Container::cxxDeallocate(size_t n, T* ptr) noexcept
{
  for (size_t i = 0; i < n; ++i)
  {
    destructorAt(ptr + i);
  }
  deallocate((void*)(ptr));
}

}