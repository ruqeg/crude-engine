#pragma once

#include <core/memory/iallocator.hpp>
#include <core/memory/memory_utils.hpp>

namespace crude_engine
{

class CXX_Allocator : public IAllocator
{
public:
  CRUDE_NODISCARD void* allocate(std::size_t size) noexcept override;
  void free(void* ptr) noexcept override;

  template<class T, typename... Args>
  CRUDE_NODISCARD T* mnew(Args&&... args) noexcept;

  template<class T, typename... Args>
  CRUDE_NODISCARD T* mnewArray(std::size_t n, Args&&... args) noexcept;

  template<class T>
  void mdelete(T* ptr) noexcept;

  template<class T>
  void mdeleteArray(std::size_t n, T* ptr) noexcept;
};

template<class T, typename... Args>
T* CXX_Allocator::mnew(Args&&... args) noexcept
{
  return Memory_Utils::constructAt(allocate(sizeof(T)), Utility::forward<Args>(args)...);
}

template<class T, typename... Args>
T* CXX_Allocator::mnewArray(std::size_t n, Args&&... args) noexcept
{
  T* ptr = reinterpret_cast<T*>(allocate(sizeof(T) * n));

  for (std::size_t i = 0; i < n; ++i)
  {
    Memory_Utils::constructAt(ptr + i, Utility::forward<Args>(args)...);
  }
  return ptr;
}

template<class T>
void CXX_Allocator::mdelete(T* ptr) noexcept
{
  Memory_Utils::destructorAt(ptr);
  free(reinterpret_cast<void*>(ptr));
}

template<class T>
void CXX_Allocator::mdeleteArray(std::size_t n, T* ptr) noexcept
{
  for (std::size_t i = 0; i < n; ++i)
  {
    Memory_Utils::destructorAt(ptr + i);
  }
  free(reinterpret_cast<void*>(ptr));
}

}