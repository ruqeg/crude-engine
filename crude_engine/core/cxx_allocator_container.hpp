#pragma once

#include <core/iallocator.hpp>
#include <core/utility.hpp>

namespace crude_engine
{

class CXX_Allocator_Container : public IAllocator
{
public:
  template<class T, typename... Args>
  CRUDE_NODISCARD T* cxxAllocate(Args&&... args) noexcept;

  template<class T, typename... Args>
  CRUDE_NODISCARD T* cxxAllocate(std::size_t n, Args&&... args) noexcept;

  template<class T>
  void cxxDeallocate(T* ptr) noexcept;

  template<class T>
  void cxxDeallocate(std::size_t n, T* ptr) noexcept;
};

template<class T, typename ...Args>
CRUDE_NODISCARD T* CXX_Allocator_Container::cxxAllocate(Args&& ...args) noexcept
{
  return Utility::constructAt(allocate(sizeof(T)), Utility::forward<Args>(args)...);
}

template<class T, typename ...Args>
CRUDE_NODISCARD T* CXX_Allocator_Container::cxxAllocate(std::size_t n, Args && ...args) noexcept
{
  T* ptr = reinterpret_cast<T*>(allocate(sizeof(T) * n));

  for (std::size_t i = 0; i < n; ++i)
  {
    Utility::constructAt(ptr + i, Utility::forward<Args>(args)...);
  }
  return ptr;
}

template<class T>
void CXX_Allocator_Container::cxxDeallocate(T* ptr) noexcept
{
  Utility::destructorAt(ptr);
  deallocate(reinterpret_cast<void*>(ptr));
}

template<class T>
void CXX_Allocator_Container::cxxDeallocate(std::size_t n, T* ptr) noexcept
{
  for (std::size_t i = 0; i < n; ++i)
  {
    Utility::destructorAt(ptr + i);
  }
  deallocate(reinterpret_cast<void*>(ptr));
}

}

#include <core/cxx_allocator_container.inl>