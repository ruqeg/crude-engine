module crude.core.free_rbt_allocator;

import crude.core.logger;
import crude.core.assert;
import crude.core.memory_utils;

namespace crude::core
{

bool operator<(const Free_RBT_Allocator::Node& lhs, int rhs)
{
  return lhs.blockSize < rhs;
}

Free_RBT_Allocator::Free_RBT_Allocator(const size_t capacity, Placement_Policy placementPolicy /*= PLACEMANT_POLICY_FIND_BEST*/) noexcept
  :
  m_capacity(capacity),
  m_policy(placementPolicy)
{
  if (m_heap != nullptr)
  {
    Memory_Utils::free(m_heap);
  }

  m_heap = reinterpret_cast<byte*>(Memory_Utils::allocate(capacity));

  reset();
}

Free_RBT_Allocator::~Free_RBT_Allocator() noexcept
{
  if (m_rbt.begin()->blockSize != m_capacity)
  {
    logInfo(Debug::Channel::Memory, "[Memory leak] Free_RBT_Allocator::~Free_RBT_Allocator\n\tm_capacity: %i\n\tblockSize: %i\n\tm_capacity != blockSize\n", m_capacity, m_rbt.begin()->blockSize);
  }

  assert("Memory leak" && (m_rbt.size() == 1));
  assert("Memory leak" && (m_rbt.begin()->blockSize == m_capacity));

  Memory_Utils::free(m_heap);
  m_heap = nullptr;
}

void* Free_RBT_Allocator::allocate(size_t size) noexcept
{
  const size_t requiredSize = static_cast<size_t>(size + sizeof(Node));

  logInfo(Debug::Channel::Memory, "Free_RBT_Allocator::allocate() blockSize: %i", requiredSize);

  // !TODO
  //CRUDE_ASSERT("Allocation size must be bigger" && size >= sizeof(Node));
  const Red_Black_Tree<Node>::Iterator& allocatedHeaderIt = m_rbt.lowerBound(requiredSize);
  
  size_t(allocatedHeaderIt->free);
  size_t("The allocator is full" && allocatedHeaderIt != nullptr);

  Node* allocatedHeader = &*allocatedHeaderIt;
  byte* allocatedHeaderAddress = reinterpret_cast<byte*>(allocatedHeader);

  byte* resultAddress = allocatedHeaderAddress + sizeof(Node);
  
  const size_t remainingBlockSize = allocatedHeader->blockSize - requiredSize;

  if (remainingBlockSize > sizeof(Node))
  {
    Node* newFreeHeader = reinterpret_cast<Node*>(resultAddress + size);
    *newFreeHeader = Node(remainingBlockSize, true, allocatedHeader, allocatedHeader->next);
    if (newFreeHeader->next)
    {
      newFreeHeader->next->prev = newFreeHeader;
    }

    m_rbt.remove(*allocatedHeader);
    *allocatedHeader = Node(requiredSize, false, allocatedHeader->prev, newFreeHeader);

    m_rbt.insert(*newFreeHeader);
  }
  else
  {
    m_rbt.remove(*allocatedHeader);
    *allocatedHeader = Node(allocatedHeader->blockSize, false, allocatedHeader->prev, allocatedHeader->next);
  }

  if constexpr (cEnableAsserts)
  {
    assertLostMemoryBlock();
    assertCorruptedMemoryList();
    assertUnmergedMemoryBlocks();
  }

  return resultAddress;
}

void Free_RBT_Allocator::deallocate(void* ptr) noexcept
{
  assert(ptr);

  byte* allocatedAddress = reinterpret_cast<byte*>(ptr);
  byte* allocatedHeaderAddress = allocatedAddress - sizeof(Node);
  Node* allocatedHeader = reinterpret_cast<Node*>(allocatedHeaderAddress);

  assert(!allocatedHeader->free && !allocatedHeader->getParent() && !allocatedHeader->getLeft() && !allocatedHeader->getRight());
  logInfo(Debug::Channel::Memory, "Free_RBT_Allocator::deallocate() blockSize: %i", allocatedHeader->blockSize);

  *allocatedHeader = Node(allocatedHeader->blockSize, true, allocatedHeader->prev, allocatedHeader->next);

  if (allocatedHeader->prev && (allocatedHeader->prev->free))
  {
    assert(allocatedHeader->prev->next == allocatedHeader);

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
    assert(allocatedHeader->next->prev == allocatedHeader);

    m_rbt.remove(*allocatedHeader->next);

    *allocatedHeader = Node(allocatedHeader->blockSize + allocatedHeader->next->blockSize, true, allocatedHeader->prev, allocatedHeader->next->next);
    if (allocatedHeader->next)
    {
      allocatedHeader->next->prev = allocatedHeader;
    }
  }

  m_rbt.insert(*allocatedHeader);

  if constexpr (cEnableAsserts)
  {
    assertLostMemoryBlock();
    assertCorruptedMemoryList();
    assertUnmergedMemoryBlocks();
  }

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
  assert([this]() -> bool {
    Node* header = &*m_rbt.begin();
    while (header->prev)
    {
      header = header->prev;
    }

    size_t totalBlockSize = 0u;
    while (header)
    {
      totalBlockSize += header->blockSize;
      header = header->next;
    }

    if (m_capacity != totalBlockSize)
    {
      logError(
        Debug::Channel::Memory,
        "Free_RBT_Allocator::assertLostMemoryBlock.\n"
        "\tm_capacity: %i\n"
        "\ttotalBlockSize: %i\n",
        m_capacity, totalBlockSize);
    }

    return (m_capacity == totalBlockSize);
    }());
}

void Free_RBT_Allocator::assertCorruptedMemoryList()
{
  assert([this]() -> bool {
    Node* header = &*m_rbt.begin();
    while (header->prev)
    {
      header = header->prev;
    }

    while (header)
    {
      if ((header->next && (header->next->prev != header)) || (header->prev && (header->prev->next != header)))
      {
        return false;
      }

      header = header->next;
    }

    return true;
    }());
}

void Free_RBT_Allocator::assertUnmergedMemoryBlocks()
{
  assert([this]() -> bool {
    for (auto it = m_rbt.begin(); it != m_rbt.end(); ++it)
    {
      if ((it->next && it->free && it->next->free) || (it->prev && it->free && it->prev->free))
      {
        logError(
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
}

}