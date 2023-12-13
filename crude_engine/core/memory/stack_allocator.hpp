#pragma once

#include <core/alias.hpp>

namespace crude_engine
{

class Stack_Allocator
{
public:
  Stack_Allocator(const std::size_t capacity) noexcept;
  ~Stack_Allocator() noexcept;
  CRUDE_NODISCARD void* allocate(std::size_t size) noexcept;
  void pop() noexcept;
  void reset() noexcept;
protected:
  std::byte*         m_heap{ nullptr};
  std::size_t        m_heapSize;
  const std::size_t  m_capacity;
protected:
  struct Allocation_Header 
  {
    std::size_t padding;
  };
};

}