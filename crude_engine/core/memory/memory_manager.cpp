#include <core/memory/memory_manager.hpp>

namespace crude_engine
{

Free_RBT_Allocator& Memory_Manager::getFreeRBTAllocators() noexcept
{
  return m_freeRBTAllocators;
}

Memory_Manager& Memory_Manager::getInstance() noexcept
{
  static Memory_Manager instance;
  return instance;
}

} // namespace crude_engine