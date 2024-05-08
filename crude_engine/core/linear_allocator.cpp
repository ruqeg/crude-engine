module linear_allocator;

import crude_engine.memory_utils;
import crude_engine.assert;

namespace crude_engine
{

Linear_Allocator::Linear_Allocator(const size_t capacity) noexcept
  :
  m_capacity(capacity)
{
  if (m_heap != nullptr)
  {
    Memory_Utils::free(m_heap);
  }

  m_heap = reinterpret_cast<byte*>(Memory_Utils::allocate(capacity));
  m_heapSize = 0u;
}

Linear_Allocator::~Linear_Allocator()
{
  Memory_Utils::free(m_heap);
  m_heap = nullptr;
}

void* Linear_Allocator::allocate(size_t size) noexcept
{
  assert(m_heapSize + size <= m_capacity);

  void* resultPtr = m_heap + m_heapSize;
  m_heapSize += size;
  return resultPtr;
}

void Linear_Allocator::reset() noexcept
{
  m_heapSize = 0u;
}

}