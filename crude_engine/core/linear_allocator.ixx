module;

export module linear_allocator;

import crude_engine.iallocator;

export namespace crude_engine
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