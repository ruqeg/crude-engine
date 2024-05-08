module;

export module crude_engine.pool_allocator;

import crude_engine.iallocator;
import crude_engine.stack_linked_list;

export namespace crude_engine
{

class Pool_Allocator : public IAllocator
{
public:
  struct Free_Header {};
  using Node = Stack_Linked_List<Free_Header>::Node;

public:
  Pool_Allocator(const size_t capacity, const size_t chunkSize) noexcept;
  ~Pool_Allocator() noexcept;
  [[nodiscard]] void* allocate(size_t size) noexcept override;
  void deallocate(void* ptr) noexcept override;
  void reset() noexcept;

protected:
  byte*                           m_heap{ nullptr};
  size_t                          m_heapSize;
  const size_t                    m_capacity;
  const size_t                    m_chunkSize;
  Stack_Linked_List<Free_Header>  m_freeList;
};

}