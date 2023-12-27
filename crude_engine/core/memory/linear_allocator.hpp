#pragma once

#include <core/alias.hpp>

namespace crude_engine
{

class Linear_Allocator
{
public:
  Linear_Allocator(const std::size_t capacity) noexcept;
  ~Linear_Allocator() noexcept;
  CRUDE_NODISCARD void* allocate(std::size_t size) noexcept;
  void reset() noexcept;
protected:
  std::byte*         m_heap{nullptr};
  std::size_t        m_heapSize;
  const std::size_t  m_capacity;
};

}