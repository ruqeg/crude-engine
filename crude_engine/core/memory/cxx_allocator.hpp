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

}

#include <core/memory/cxx_allocator.inl>