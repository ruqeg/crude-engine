#pragma once

#include <core/memory/iallocator_a.hpp>

namespace crude_engine
{

class Linear_Allocator : IAllocator_A
{
public:
  Linear_Allocator(const std::size_t capacity) noexcept;
  ~Linear_Allocator() noexcept;
  CRUDE_NODISCARD void* allocate(std::size_t size) noexcept override;
  void reset() noexcept;
protected:
  std::byte*         m_heap{nullptr};
  std::size_t        m_heapSize;
  const std::size_t  m_capacity;
};

}