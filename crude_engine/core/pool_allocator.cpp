module crude_engine.core.pool_allocator;

 import crude_engine.core.memory_utils;
 import crude_engine.core.assert;

namespace crude_engine
{

Pool_Allocator::Pool_Allocator(const size_t capacity, const size_t chunkSize) noexcept
  :
  m_capacity(capacity),
  m_chunkSize(chunkSize)
{
  assert(chunkSize >= 8 && "Chunk size must be greater or equal to 8");
  assert(capacity % chunkSize == 0 && "Capacity must be a multiple of Chunk Size");

  if (m_heap != nullptr)
  {
    Memory_Utils::free(m_heap);
  }

  m_heap = reinterpret_cast<byte*>(Memory_Utils::allocate(capacity));

  reset();
}

Pool_Allocator::~Pool_Allocator() noexcept
{
  Memory_Utils::free(m_heap);
  m_heap = nullptr;
}

void* Pool_Allocator::allocate(size_t size) noexcept
{
  assert(size == this->m_chunkSize && "Allocation size must be equal to chunk size");

  Node* resultPtr = m_freeList.pop();
  assert(resultPtr != nullptr && "The pool allocator is full");

  m_heapSize += m_chunkSize;

  return resultPtr;
}

void Pool_Allocator::deallocate(void* ptr) noexcept
{
  assert(ptr);

  m_heapSize -= m_chunkSize;
  m_freeList.push(reinterpret_cast<Node*>(ptr));
}

void Pool_Allocator::reset() noexcept
{
  m_heapSize = 0u;

  const size_t chunksNum = m_capacity / m_chunkSize;
  for (size_t i = 0; i < chunksNum; ++i)
  {
    const size_t address = (size_t)m_heap + i * m_chunkSize;
    m_freeList.push(reinterpret_cast<Node*>(address));
  }
}

}