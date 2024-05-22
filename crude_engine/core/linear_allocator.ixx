module;

export module crude.core.linear_allocator;

import crude.core.iallocator;

export namespace crude::core
{

class Linear_Allocator : public IAllocator_Allocate
{
public:
  Linear_Allocator(const size_t capacity) noexcept;
  ~Linear_Allocator() noexcept;
  [[nodiscard]] void* allocate(size_t size) noexcept override;
  void reset() noexcept;

protected:
  byte*         m_heap{nullptr};
  size_t        m_heapSize;
  const size_t  m_capacity;
};

}