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
  auto allocatedNode = m_rbt.lowerBound(requiredSize);
  
  CRUDE_ASSERT("The allocator is full" && allocatedNode != nullptr);

  byte* resultPtr = reinterpret_cast<byte*>(&*allocatedNode) + sizeof(Node);

  Node* newFreeNode = reinterpret_cast<Node*>(resultPtr + size);
  *newFreeNode = {};

  newFreeNode->blockSize = allocatedNode->blockSize - requiredSize;
  newFreeNode->free = true;
  newFreeNode->prev = &*allocatedNode;
  newFreeNode->next = nullptr;

  allocatedNode->free = false;
  allocatedNode->blockSize = requiredSize;
  allocatedNode->next = newFreeNode;

  m_rbt.remove(*allocatedNode);
  m_rbt.insert(*newFreeNode);

  return resultPtr;
}

void Free_RBT_Allocator::free(void* ptr) noexcept
{
  const std::size_t currentAddress = (std::size_t)ptr;
  const std::size_t headerAddress = currentAddress - sizeof(Node);
  Node* allocatedNode = reinterpret_cast<Node*>(headerAddress);

  if (allocatedNode->prev && (allocatedNode->prev->free))
  {
    m_rbt.remove(*allocatedNode->prev);

    allocatedNode->prev->blockSize += allocatedNode->blockSize;
    allocatedNode->prev->next = allocatedNode->next;
    if (allocatedNode->prev->next)
    {
      allocatedNode->prev->next->prev = allocatedNode->prev;
    }

    allocatedNode = allocatedNode->prev;
  }

  if (allocatedNode->next && (allocatedNode->next->free))
  {
    m_rbt.remove(*allocatedNode->next);
    allocatedNode->blockSize += allocatedNode->next->blockSize;

    allocatedNode->next = allocatedNode->next->next;
    if (allocatedNode->next)
    {
      allocatedNode->next->prev = allocatedNode;
    }
  }

  allocatedNode->free = true;
  m_rbt.insert(*allocatedNode);
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