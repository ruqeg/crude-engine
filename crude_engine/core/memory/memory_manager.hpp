#pragma once

#include <core/data_structures/array_stack.hpp>
#include <core/memory/free_rbt_allocator.hpp>

namespace crude_engine
{

class Memory_Manager
{
public:
  Free_RBT_Allocator& getFreeRBTAllocators() noexcept;

public:
  static Memory_Manager& getInstance() noexcept;

private:
  Memory_Manager() = default;
  ~Memory_Manager() = default;
  
private:
  static constexpr std::size_t FREE_RBT_ALLOCATOR_CAPCITY = 10000000u;

private:
  Free_RBT_Allocator m_freeRBTAllocators = Free_RBT_Allocator(FREE_RBT_ALLOCATOR_CAPCITY, Free_RBT_Allocator::PLACEMANT_POLICY_FIND_BEST);
};

}  // namespace crude_engine