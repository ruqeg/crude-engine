#include <core/memory/free_rbt_allocator.hpp>
#include <core/debug/assert.hpp>
#include <core/memory/memory_utils.hpp>

namespace crude_engine
{

bool operator<(const Free_RBT_Allocator::Node& lhs, int rhs)
{
  return lhs.blockSize < rhs;
}

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
  const std::size_t requiredSize = static_cast<std::size_t>(size + sizeof(Node));

  CRUDE_ASSERT("Allocation size must be bigger" && size >= sizeof(Node));
  const Red_Black_Tree<Node>::Iterator& allocatedHeaderIt = m_rbt.lowerBound(requiredSize);
  
  CRUDE_ASSERT("The allocator is full" && allocatedHeaderIt != nullptr);

  Node* allocatedHeader = &*allocatedHeaderIt;
  byte* allocatedHeaderAddress = reinterpret_cast<byte*>(allocatedHeader);

  byte* resultAddress = allocatedHeaderAddress + sizeof(Node);
  
  Node* newFreeHeader = reinterpret_cast<Node*>(resultAddress + size);
  *newFreeHeader = {};

  newFreeHeader->blockSize = allocatedHeader->blockSize - requiredSize;
  newFreeHeader->free = true;
  newFreeHeader->prev = allocatedHeader;
  newFreeHeader->next = nullptr;

  allocatedHeader->free = false;
  allocatedHeader->blockSize = requiredSize;
  allocatedHeader->next = newFreeHeader;

  m_rbt.remove(*allocatedHeader);
  m_rbt.insert(*newFreeHeader);

  return resultAddress;
}

void Free_RBT_Allocator::free(void* ptr) noexcept
{
  byte* allocatedAddress = reinterpret_cast<byte*>(ptr);
  byte* allocatedHeaderAddress = allocatedAddress - sizeof(Node);
  Node* allocatedHeader = reinterpret_cast<Node*>(allocatedHeaderAddress);

  if (allocatedHeader->prev && (allocatedHeader->prev->free))
  {
    m_rbt.remove(*allocatedHeader->prev);

    allocatedHeader->prev->blockSize += allocatedHeader->blockSize;
    allocatedHeader->prev->next = allocatedHeader->next;
    if (allocatedHeader->prev->next)
    {
      allocatedHeader->prev->next->prev = allocatedHeader->prev;
    }

    allocatedHeader = allocatedHeader->prev;
  }

  if (allocatedHeader->next && (allocatedHeader->next->free))
  {
    m_rbt.remove(*allocatedHeader->next);
    allocatedHeader->blockSize += allocatedHeader->next->blockSize;

    allocatedHeader->next = allocatedHeader->next->next;
    if (allocatedHeader->next)
    {
      allocatedHeader->next->prev = allocatedHeader;
    }
  }

  allocatedHeader->free = true;
  m_rbt.insert(*allocatedHeader);

  return;
}

void Free_RBT_Allocator::reset() noexcept
{
  m_heapSize = 0;

  Node* firstNode = reinterpret_cast<Node*>(m_heap);

  *firstNode = {};

  firstNode->blockSize = m_capacity;
  firstNode->free = true;
  firstNode->prev = nullptr;
  firstNode->next = nullptr;
  
  m_rbt.insert(*firstNode);
}

}