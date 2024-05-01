#pragma once

#include <core/cxx_allocator_container.hpp>
#include <core/rb_tree.hpp>
#include <core/logger.hpp>

namespace crude_engine
{

class Free_RBT_Allocator : public CXX_Allocator_Container
{
public:
  enum Placement_Policy
  {
    PLACEMANT_POLICY_FIND_FIRST,
    PLACEMANT_POLICY_FIND_BEST
  };

  struct Node : public RBT_Node_Base<Node>
  {
    Node(std::size_t blockSize, bool64 free, Node* prev, Node* next) noexcept
      :
      RBT_Node_Base<Node>(),
      next(next),
      prev(prev),
      blockSize(blockSize),
      free(free)
    {}
    Node*       next;
    Node*       prev;
    std::size_t blockSize;
    bool64      free;
    bool operator<(const Node& other) const
    {
      return blockSize < other.blockSize;
    }
  };

public:
  // !TODO make for PLACEMANT_POLICY_FIND_BEST
  Free_RBT_Allocator(const std::size_t capacity, Placement_Policy placementPolicy = PLACEMANT_POLICY_FIND_BEST) noexcept;
  ~Free_RBT_Allocator() noexcept;
  CRUDE_NODISCARD void* allocate(std::size_t size) noexcept override; // O(log(n))
  void deallocate(void* ptr) noexcept override; // O(log(n))
  void reset() noexcept;

private:
  void assertUnfreeMemory();
  void assertUnmergedMemoryBlocks();

private:
  std::byte*            m_heap{ nullptr };
  std::size_t           m_heapSize;
  const std::size_t     m_capacity;
  Red_Black_Tree<Node>  m_rbt;
  Placement_Policy      m_pPolicy;

private:
#ifdef _CRUDE_FREE_RBT_ALLOCATOR_MASSIVE_ASSERT
  std::size_t           m_unfree;
#endif
};

}