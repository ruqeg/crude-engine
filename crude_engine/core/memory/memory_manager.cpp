#include <core/memory/memory_manager.hpp>

namespace crude_engine
{

Memory_Manager::Memory_Manager()
  :
  m_defaultFreeRBTAllocators(FREE_RBT_ALLOCATOR_CAPCITY, Free_RBT_Allocator::PLACEMANT_POLICY_FIND_BEST)
{}

Free_RBT_Allocator& Memory_Manager::getDefaultFreeRBTAllocators() noexcept
{
  return m_defaultFreeRBTAllocators;
}

Memory_Manager& Memory_Manager::getInstance() noexcept
{
  static Memory_Manager instance;
  return instance;
}

} // namespace crude_engine