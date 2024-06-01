#include <memory>

module crude.core.memory_manager;

namespace crude::core
{

Memory_Manager& Memory_Manager::getInstance() noexcept
{
  static Memory_Manager instance;
  return instance;
}

void Memory_Manager::initialize(core::uint32 defaultFreeRBTCapacity) noexcept
{
  m_defaultAlignedAllocator = std::make_shared<Aligned_Allocator>();
  m_defaultFreeRBTAllocators = std::make_shared<Free_RBT_Allocator>(defaultFreeRBTCapacity, Free_RBT_Allocator::PLACEMANT_POLICY_FIND_BEST);
  m_defaultSystemAllocators = std::make_shared<System_Allocator>();
}

void Memory_Manager::deinitialize() noexcept
{
  m_defaultAlignedAllocator.reset();
  m_defaultFreeRBTAllocators.reset();
  m_defaultSystemAllocators.reset();
}

} // namespace crude