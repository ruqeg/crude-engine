module;

export module crude_engine.stack_allocator;

import crude_engine.iallocator;

export namespace crude_engine
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