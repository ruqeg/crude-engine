#pragma once

#include <core/data_structures/stack_linked_list.hpp>

namespace crude_engine
{

class Pool_Allocator
{
public:
  struct Free_Header {};
  using Node = Stack_Linked_List<Free_Header>::Node;
public:
  Pool_Allocator(const std::size_t capacity, const std::size_t chunkSize) noexcept;
  ~Pool_Allocator() noexcept;
  CRUDE_NODISCARD void* allocate(std::size_t size) noexcept;
  void free(void* ptr) noexcept;
  void reset() noexcept;
protected:
  std::byte*                      m_heap{ nullptr};
  std::size_t                     m_heapSize;
  const std::size_t               m_capacity;
  const std::size_t               m_chunkSize;
  Stack_Linked_List<Free_Header>  m_freeList;
};

}