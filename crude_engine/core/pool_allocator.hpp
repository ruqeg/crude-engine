#pragma once

#include <core/iallocator.hpp>
#include <core/memory_utils.hpp>
#include <core/stack_linked_list.hpp>

namespace crude_engine
{

class Pool_Allocator : public IAllocator
{
public:
  struct Free_Header {};
  using Node = Stack_Linked_List<Free_Header>::Node;
public:
  Pool_Allocator(const std::size_t capacity, const std::size_t chunkSize) noexcept;
  ~Pool_Allocator() noexcept;
  CRUDE_NODISCARD void* allocate(std::size_t size) noexcept override;
  void deallocate(void* ptr) noexcept override;
  void reset() noexcept;

protected:
  std::byte*                      m_heap{ nullptr};
  std::size_t                     m_heapSize;
  const std::size_t               m_capacity;
  const std::size_t               m_chunkSize;
  Stack_Linked_List<Free_Header>  m_freeList;
};

}