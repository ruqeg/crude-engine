#pragma once

#include <core/stl/rb_tree.hpp>
#include <core/memory/iallocator_a.hpp>
#include <core/memory/iallocator_f.hpp>

namespace crude_engine
{

class Free_RBT_Allocator : public IAllocator_A, public IAllocator_F
{
public:
  enum Placement_Policy
  {
    PLACEMANT_POLICY_FIND_FIRST,
    PLACEMANT_POLICY_FIND_BEST
  };
  struct Node : public RBT_Node_Base<Node>
  {
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
  Free_RBT_Allocator(const std::size_t capacity, Placement_Policy placementPolicy) noexcept;
  ~Free_RBT_Allocator() noexcept;
  CRUDE_NODISCARD void* allocate(std::size_t size) noexcept override; // O(log(n))
  void free(void* ptr) noexcept override; // O(log(n))
  void reset() noexcept;
protected:
  std::byte*            m_heap{ nullptr };
  std::size_t           m_heapSize;
  const std::size_t     m_capacity;
  Red_Black_Tree<Node>  m_rbt;
  Placement_Policy      m_pPolicy;
};

}