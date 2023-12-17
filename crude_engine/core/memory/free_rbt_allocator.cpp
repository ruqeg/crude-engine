#include <core/memory/free_rbt_allocator.hpp>
#include <core/debug/assert.hpp>
#include <core/memory/memory_utils.hpp>

namespace crude_engine
{

Free_RBT_Allocator::Free_RBT_Allocator(const std::size_t capacity, Placement_Policy placementPolicy) noexcept
  :
  m_capacity(capacity),
  m_pPolicy(placementPolicy)
{
  if (m_heap != nullptr)
  {
    Memory_Utils::free(m_heap);
  }

  m_heap = reinterpret_cast<std::byte*>(Memory_Utils::allocate(capacity));

  reset();
}

Free_RBT_Allocator::~Free_RBT_Allocator() noexcept
{
  Memory_Utils::free(m_heap);
  m_heap = nullptr;
}

void* Free_RBT_Allocator::allocate(std::size_t size) noexcept
{
  CRUDE_ASSERT("Allocation size must be bigger" && size >= sizeof(Free_Header_Node));
}

void Free_RBT_Allocator::free(void* ptr) noexcept
{

}

void Free_RBT_Allocator::reset() noexcept
{
  m_heapSize = 0;
  Free_Header_Node* firstNode = (Free_Header_Node*)m_capacity;
  firstNode->blockSize = m_capacity;
  m_rbt.insert(*firstNode);
}

}