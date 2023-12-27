#pragma once

#include <core/stl/stack_linked_list.hpp>
#include <core/memory/iallocator_a.hpp>
#include <core/memory/iallocator_f.hpp>

namespace crude_engine
{

class Pool_Allocator : public IAllocator_A, public IAllocator_F
{
public:
  struct Free_Header {};
  using Node = Stack_Linked_List<Free_Header>::Node;
public:
  Pool_Allocator(const std::size_t capacity, const std::size_t chunkSize) noexcept;
  ~Pool_Allocator() noexcept;
  CRUDE_NODISCARD void* allocate(std::size_t size) noexcept override;
  void free(void* ptr) noexcept override;
  void reset() noexcept;
protected:
  std::byte*                      m_heap{ nullptr};
  std::size_t                     m_heapSize;
  const std::size_t               m_capacity;
  const std::size_t               m_chunkSize;
  Stack_Linked_List<Free_Header>  m_freeList;
};

}