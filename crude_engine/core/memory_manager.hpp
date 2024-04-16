#pragma once

#include <core/array_stack.hpp>
#include <core/free_rbt_allocator.hpp>
#include <core/pool_allocator.hpp>
#include <core/cxx_allocator.hpp>

namespace crude_engine
{

class Memory_Manager
{
public:
  Free_RBT_Allocator& getDefaultFreeRBTAllocators() noexcept;
  CXX_Allocator& getDefaultCxxAllocators() noexcept;

public:
  static Memory_Manager& getInstance() noexcept;

private:
  Memory_Manager();
  ~Memory_Manager() = default;
  
private:
  static constexpr std::size_t FREE_RBT_ALLOCATOR_CAPCITY = 10000000u;

private:
  Free_RBT_Allocator m_defaultFreeRBTAllocators;
  CXX_Allocator m_defaultCxxAllocators;
};

}  // namespace crude_engine