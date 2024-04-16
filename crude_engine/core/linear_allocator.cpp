#include <core/linear_allocator.hpp>
#include <core/memory_utils.hpp>
#include <core/assert.hpp>

namespace crude_engine
{

Linear_Allocator::Linear_Allocator(const std::size_t capacity) noexcept
  :
  m_capacity(capacity)
{
  if (m_heap != nullptr)
  {
    Memory_Utils::free(m_heap);
  }

  m_heap = reinterpret_cast<std::byte*>(Memory_Utils::allocate(capacity));
  m_heapSize = 0u;
}

Linear_Allocator::~Linear_Allocator()
{
  Memory_Utils::free(m_heap);
  m_heap = nullptr;
}

void* Linear_Allocator::allocate(std::size_t size) noexcept
{
  CRUDE_ASSERT(m_heapSize + size <= m_capacity);

  void* resultPtr = m_heap + m_heapSize;
  m_heapSize += size;
  return resultPtr;
}

void Linear_Allocator::reset() noexcept
{
  m_heapSize = 0u;
}

}