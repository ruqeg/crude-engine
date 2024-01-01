#pragma once

#include <core/memory/iallocator.hpp>
#include <core/data_structures/rb_tree.hpp>

namespace crude_engine
{

class Free_RBT_Allocator : public IAllocator
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

  template<class T, typename... Args>
  CRUDE_NODISCARD T* mnew(std::size_t n, Args&&... args) noexcept;

  template<class T>
  void mdelete(T* ptr) noexcept;

protected:
  std::byte*            m_heap{ nullptr };
  std::size_t           m_heapSize;
  const std::size_t     m_capacity;
  Red_Black_Tree<Node>  m_rbt;
  Placement_Policy      m_pPolicy;
};

}

#include <core/memory/free_rbt_allocator.inl>