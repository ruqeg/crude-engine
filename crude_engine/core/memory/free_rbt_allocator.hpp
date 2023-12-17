#pragma once

#include <core/stl/rb_tree.hpp>

namespace crude_engine
{

class Free_RBT_Allocator
{
public:
  enum Placement_Policy
  {
    PLACEMANT_POLICY_FIND_FIRST,
    PLACEMANT_POLICY_FIND_BEST
  };
  struct Free_Header_Node : public RBT_Node_Base<Free_Header_Node>
  {
    std::size_t blockSize;
  };
public:
  Free_RBT_Allocator(const std::size_t capacity, Placement_Policy placementPolicy) noexcept;
  ~Free_RBT_Allocator() noexcept;
  CRUDE_NODISCARD void* allocate(std::size_t size) noexcept;
  void free(void* ptr) noexcept;
  void reset() noexcept;
protected:
  std::byte*                        m_heap{ nullptr };
  std::size_t                       m_heapSize;
  const std::size_t                 m_capacity;
  Red_Black_Tree<Free_Header_Node>  m_rbt;
  Placement_Policy                  m_pPolicy;
};

}