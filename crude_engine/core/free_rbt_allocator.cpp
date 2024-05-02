#include <core/free_rbt_allocator.hpp>
#include <core/assert.hpp>
#include <core/memory_utils.hpp>

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
  if (m_rbt.begin()->blockSize != m_capacity)
  {
    CRUDE_LOG_INFO(Debug::Channel::Memory, "[Memory leak] Free_RBT_Allocator::~Free_RBT_Allocator\n\tm_capacity: %i\n\tblockSize: %i\n\tm_capacity != blockSize\n", m_capacity, m_rbt.begin()->blockSize);
  }

  CRUDE_ASSERT("Memory leak" && (m_rbt.size() == 1));
  CRUDE_ASSERT("Memory leak" && (m_rbt.begin()->blockSize == m_capacity));

  Memory_Utils::free(m_heap);
  m_heap = nullptr;
}

void* Free_RBT_Allocator::allocate(std::size_t size) noexcept
{
  static int counter = 0u;
  
  if (counter == 5)
  {
    static int a = 0;
    a++;
  }
  
  const std::size_t requiredSize = static_cast<std::size_t>(size + sizeof(Node));

  CRUDE_LOG_INFO(Debug::Channel::Memory, "Free_RBT_Allocator::allocate() blockSize: %i", requiredSize);

  // !TODO
  //CRUDE_ASSERT("Allocation size must be bigger" && size >= sizeof(Node));
  const Red_Black_Tree<Node>::Iterator& allocatedHeaderIt = m_rbt.lowerBound(requiredSize);
  
  CRUDE_ASSERT(allocatedHeaderIt->free);
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
  counter++;

#ifdef _CRUDE_FREE_RBT_ALLOCATOR_MASSIVE_ASSERT
  assertLostMemoryBlock();
  assertCorruptedMemoryList();
  assertUnmergedMemoryBlocks();
#endif

  return resultAddress;
}

void Free_RBT_Allocator::deallocate(void* ptr) noexcept
{
  CRUDE_ASSERT(ptr);

  byte* allocatedAddress = reinterpret_cast<byte*>(ptr);
  byte* allocatedHeaderAddress = allocatedAddress - sizeof(Node);
  Node* allocatedHeader = reinterpret_cast<Node*>(allocatedHeaderAddress);

  CRUDE_ASSERT(!allocatedHeader->free && !allocatedHeader->getParent() && !allocatedHeader->getLeft() && !allocatedHeader->getRight());
  CRUDE_LOG_INFO(Debug::Channel::Memory, "Free_RBT_Allocator::deallocate() blockSize: %i", allocatedHeader->blockSize);

  *allocatedHeader = Node(allocatedHeader->blockSize, true, allocatedHeader->prev, allocatedHeader->next);

  if (allocatedHeader->prev && (allocatedHeader->prev->free))
  {
    CRUDE_ASSERT(allocatedHeader->prev->next == allocatedHeader);

    m_rbt.remove(*allocatedHeader->prev);

    allocatedHeader = allocatedHeader->prev;
    *allocatedHeader = Node(allocatedHeader->blockSize + allocatedHeader->next->blockSize, true, allocatedHeader->prev, allocatedHeader->next->next);
    if (allocatedHeader->next)
    {
      allocatedHeader->next->prev = allocatedHeader;
    }
  }
  
  if (allocatedHeader->next && (allocatedHeader->next->free))
  {
    CRUDE_ASSERT(allocatedHeader->next->prev == allocatedHeader);

    m_rbt.remove(*allocatedHeader->next);

    *allocatedHeader = Node(allocatedHeader->blockSize + allocatedHeader->next->blockSize, true, allocatedHeader->prev, allocatedHeader->next->next);
    if (allocatedHeader->next)
    {
      allocatedHeader->next->prev = allocatedHeader;
    }
  }
  
  m_rbt.insert(*allocatedHeader);

#ifdef _CRUDE_FREE_RBT_ALLOCATOR_MASSIVE_ASSERT
  assertLostMemoryBlock();
  assertCorruptedMemoryList();
  assertUnmergedMemoryBlocks();
#endif

  return;
}

void Free_RBT_Allocator::reset() noexcept
{
  m_heapSize = 0;

  Node* firstNode = reinterpret_cast<Node*>(m_heap);
  *firstNode = Node(m_capacity, true, nullptr, nullptr);
  
  m_rbt.insert(*firstNode);
}

void Free_RBT_Allocator::assertLostMemoryBlock()
{
#ifdef _CRUDE_FREE_RBT_ALLOCATOR_MASSIVE_ASSERT
  CRUDE_ASSERT([this]() -> bool {
    Node* header = &*m_rbt.begin();
    while (header->prev)
    {
      header = header->prev;
    }

    std::size_t totalBlockSize = 0u;
    while (header)
    {
      totalBlockSize += header->blockSize;
      header = header->next;
    }

    if (m_capacity != totalBlockSize)
    {
      CRUDE_LOG_INFO(
        Debug::Channel::Memory,
        "Free_RBT_Allocator::assertLostMemoryBlock.\n"
        "\tm_capacity: %i\n"
        "\ttotalBlockSize: %i\n",
        m_capacity, totalBlockSize);
    }

    return (m_capacity == totalBlockSize);
    }());
#endif
}

void Free_RBT_Allocator::assertCorruptedMemoryList()
{
#ifdef _CRUDE_FREE_RBT_ALLOCATOR_MASSIVE_ASSERT
  CRUDE_ASSERT([this]() -> bool {
    Node* header = &*m_rbt.begin();
    while (header->prev)
    {
      header = header->prev;
    }

    while (header)
    {
      if (header->next && (header->next->prev != header))
      {
        return false;
      }

      header = header->next;
    }

    return true;
    }());
#endif
}

void Free_RBT_Allocator::assertUnmergedMemoryBlocks()
{
#ifdef _CRUDE_FREE_RBT_ALLOCATOR_MASSIVE_ASSERT
  CRUDE_ASSERT([this]() -> bool {
    for (auto it = m_rbt.begin(); it != m_rbt.end(); ++it)
    {
      if ((it->next && it->free && it->next->free) || (it->prev && it->free && it->prev->free))
      {
        CRUDE_LOG_INFO(
          Debug::Channel::Memory,
          "Free_RBT_Allocator::assertUnmergedMemoryBlocks.\n"
          "\tit: %#.8x. it->free: %i\n"
          "\tit->next: %#.8x. it->next->free: %i\n"
          "\tit->prev: %#.8x. it->prev->free: %i\n",
          it, it->free,
          it->next, it->next ? it->next->free : 0,
          it->prev, it->prev ? it->prev->free : 0);
        return false;
      }
    }
    return true;
    }());
#endif
}

}