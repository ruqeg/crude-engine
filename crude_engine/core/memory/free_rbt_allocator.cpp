#include <core/memory/free_rbt_allocator.hpp>
#include <core/debug/assert.hpp>
#include <core/memory/memory_utils.hpp>

namespace crude_engine
{

bool operator<(const Free_RBT_Allocator::Node& lhs, int rhs)
{
  return lhs.blockSize < rhs;
}

Free_RBT_Allocator::Free_RBT_Allocator(const std::size_t capacity, Placement_Policy placementPolicy /*= PLACEMANT_POLICY_FIND_BEST*/) noexcept
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
  CRUDE_ASSERT("Memory leak" && (m_rbt.size() == 1));
  CRUDE_ASSERT("Memory ???" && (m_rbt.begin()->blockSize == m_capacity));
  Memory_Utils::free(m_heap);
  m_heap = nullptr;
}

void* Free_RBT_Allocator::allocate(std::size_t size) noexcept
{
  const std::size_t requiredSize = static_cast<std::size_t>(size + sizeof(Node));

  CRUDE_LOG_INFO(Debug::Channel::Memory, "Free_RBT_Allocator::allocate() blockSize: %i", requiredSize);
  CRUDE_LOG_INFO(Debug::Channel::Memory, "CURRENT_MAX: %i", m_rbt.rend()->blockSize);

  // !TODO
  //CRUDE_ASSERT("Allocation size must be bigger" && size >= sizeof(Node));
  const Red_Black_Tree<Node>::Iterator& allocatedHeaderIt = m_rbt.lowerBound(requiredSize);
  
  CRUDE_ASSERT("The allocator is full" && allocatedHeaderIt != nullptr);

  Node* allocatedHeader = &*allocatedHeaderIt;
  byte* allocatedHeaderAddress = reinterpret_cast<byte*>(allocatedHeader);

  byte* resultAddress = allocatedHeaderAddress + sizeof(Node);
  
  const std::size_t remainingBlockSize = allocatedHeader->blockSize - requiredSize;

  if (remainingBlockSize > sizeof(Node))
  {
    Node* newFreeHeader = reinterpret_cast<Node*>(resultAddress + size);
    *newFreeHeader = Node(remainingBlockSize, true, allocatedHeader, allocatedHeader->next);

    m_rbt.remove(*allocatedHeader);
    *allocatedHeader = Node(requiredSize, false, allocatedHeader->prev, newFreeHeader);

    m_rbt.insert(*newFreeHeader);
  }
  else
  {
    m_rbt.remove(*allocatedHeader);
    *allocatedHeader = Node(allocatedHeader->blockSize, false, allocatedHeader->prev, allocatedHeader->next);
  }

  return resultAddress;
}

void Free_RBT_Allocator::free(void* ptr) noexcept
{
  byte* allocatedAddress = reinterpret_cast<byte*>(ptr);
  byte* allocatedHeaderAddress = allocatedAddress - sizeof(Node);
  Node* allocatedHeader = reinterpret_cast<Node*>(allocatedHeaderAddress);
  
  CRUDE_LOG_INFO(Debug::Channel::Memory, "Free_RBT_Allocator::free() blockSize: %i", allocatedHeader->blockSize);
  CRUDE_LOG_INFO(Debug::Channel::Memory, "CURRENT_MAX: %i", m_rbt.rend()->blockSize);
  
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
  *firstNode = Node(m_capacity, true, nullptr, nullptr);
  
  m_rbt.insert(*firstNode);
}

Free_RBT_Allocator::Node::Node(std::size_t blockSize, bool64 free, Node* prev, Node* next) noexcept
  :
  RBT_Node_Base<Node>(),
  next(next),
  prev(prev),
  blockSize(blockSize),
  free(free)
{}

}