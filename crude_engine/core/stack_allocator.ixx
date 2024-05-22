module;

export module crude.core.stack_allocator;

import crude.core.iallocator;

export namespace crude::core
{

class Stack_Allocator : public IAllocator_Allocate
{
protected:
  struct Allocation_Header
  {
    size_t padding;
  };

public:
  Stack_Allocator(const size_t capacity) noexcept;
  ~Stack_Allocator() noexcept;
  [[nodiscard]] void* allocate(size_t size) noexcept override;
  void pop() noexcept;
  void reset() noexcept;

protected:
  byte*         m_heap{ nullptr};
  size_t        m_heapSize;
  const size_t  m_capacity;
};

}