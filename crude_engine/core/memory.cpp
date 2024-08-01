#include <memory>

module crude.core.memory;

namespace crude::core
{

std::shared_ptr<Free_RBT_Allocator> defaultFreeRBTAllocators;
std::shared_ptr<System_Allocator> defaultSystemAllocators;
std::shared_ptr<Aligned_Allocator> defaultAlignedAllocator;


void initializeMemory(core::uint32 defaultFreeRBTCapacity) noexcept
{
  defaultAlignedAllocator = std::make_shared<Aligned_Allocator>();
  defaultFreeRBTAllocators = std::make_shared<Free_RBT_Allocator>(defaultFreeRBTCapacity, Free_RBT_Allocator::PLACEMANT_POLICY_FIND_BEST);
  defaultSystemAllocators = std::make_shared<System_Allocator>();
}

void resetMemory() noexcept
{
  defaultAlignedAllocator.reset();
  defaultFreeRBTAllocators.reset();
  defaultSystemAllocators.reset();
}

std::shared_ptr<CXX_Allocator_Container> getDefaultAllocator()
{
  return defaultFreeRBTAllocators;
}

std::shared_ptr<Free_RBT_Allocator> getDefaultFreeRBTAllocator() noexcept
{
  return defaultFreeRBTAllocators;
}

std::shared_ptr<System_Allocator> getDefaultSystemAllocator() noexcept
{
  return defaultSystemAllocators;
}

std::shared_ptr<Aligned_Allocator> getDefaultAlignedAllocator() noexcept
{
  return defaultAlignedAllocator;
}

}