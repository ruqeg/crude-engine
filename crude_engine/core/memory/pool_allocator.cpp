#include <core/memory/pool_allocator.hpp>
#include <core/memory/memory_utils.hpp>
#include <core/debug/assert.hpp>

namespace crude_engine
{

Pool_Allocator::Pool_Allocator(const std::size_t capacity, const std::size_t chunkSize) noexcept
  :
  m_capacity(capacity),
  m_chunkSize(chunkSize)
{
  CRUDE_ASSERT(chunkSize >= 8 && "Chunk size must be greater or equal to 8");
  CRUDE_ASSERT(capacity % chunkSize == 0 && "Capacity must be a multiple of Chunk Size");

  if (m_heap != nullptr)
  {
    Memory_Utils::free(m_heap);
  }

  m_heap = reinterpret_cast<std::byte*>(Memory_Utils::allocate(capacity));

  reset();
}

Pool_Allocator::~Pool_Allocator() noexcept
{
  Memory_Utils::free(m_heap);
  m_heap = nullptr;
}

void* Pool_Allocator::allocate(std::size_t size) noexcept
{
  CRUDE_ASSERT(size == this->m_chunkSize && "Allocation size must be equal to chunk size");

  Node* resultPtr = m_freeList.pop();
  CRUDE_ASSERT(resultPtr != nullptr && "The pool allocator is full");

  m_heapSize += m_chunkSize;

  return resultPtr;
}

void Pool_Allocator::free(void* ptr) noexcept
{
  CRUDE_ASSERT(ptr);

  m_heapSize -= m_chunkSize;
  m_freeList.push(reinterpret_cast<Node*>(ptr));
}

void Pool_Allocator::reset() noexcept
{
  m_heapSize = 0u;

  const std::size_t chunksNum = m_capacity / m_chunkSize;
  for (std::size_t i = 0; i < chunksNum; ++i)
  {
    const std::size_t address = (std::size_t)m_heap + i * m_chunkSize;
    m_freeList.push(reinterpret_cast<Node*>(address));
  }
}

}