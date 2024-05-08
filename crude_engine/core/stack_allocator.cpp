module crude_engine.core.stack_allocator;

import crude_engine.core.memory_utils;
import crude_engine.core.assert;

namespace crude_engine
{

Stack_Allocator::Stack_Allocator(const size_t capacity) noexcept
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

Stack_Allocator::~Stack_Allocator()
{
  Memory_Utils::free(m_heap);
  m_heap = nullptr;
}

void* Stack_Allocator::allocate(size_t size) noexcept
{
  const size_t newAddress = m_heapSize + size + sizeof(Allocation_Header);
  assert(newAddress <= m_capacity);

  const size_t headerAddress = newAddress - sizeof(Allocation_Header);

  Allocation_Header* headerPtr{ reinterpret_cast<Allocation_Header*>(m_heap + headerAddress) };
  headerPtr->padding = size;

  void* resultPtr = m_heap + m_heapSize;
  m_heapSize = newAddress;
  return resultPtr;
}

void Stack_Allocator::pop() noexcept
{
  const size_t currentAddress = (size_t)m_heapSize;
  const size_t headerAddress = currentAddress - sizeof(Allocation_Header);
  const Allocation_Header* allocationHeader{ reinterpret_cast<Allocation_Header*>(m_heap + headerAddress) };

  m_heapSize -= allocationHeader->padding + sizeof(Allocation_Header);
}

void Stack_Allocator::reset() noexcept
{
  m_heapSize = 0u;
}

}