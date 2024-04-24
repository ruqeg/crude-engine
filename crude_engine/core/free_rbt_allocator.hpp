#pragma once

#include <core/iallocator.hpp>
#include <core/rb_tree.hpp>
#include <core/logger.hpp>

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
    Node(std::size_t blockSize, bool64 free, Node* prev, Node* next) noexcept;
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

  template<class T, typename... Args>
  CRUDE_NODISCARD T* mnew(Args&&... args) noexcept;

  template<class T, typename... Args>
  CRUDE_NODISCARD T* mnewArray(std::size_t n, Args&&... args) noexcept;

  template<class T>
  void mdelete(T* ptr) noexcept;

  template<class T>
  void mdeleteArray(std::size_t n, T* ptr) noexcept;

private:
  std::byte*            m_heap{ nullptr };
  std::size_t           m_heapSize;
  const std::size_t     m_capacity;
  Red_Black_Tree<Node>  m_rbt;
  Placement_Policy      m_pPolicy;
};

}

#include <core/free_rbt_allocator.inl>