module;

#include <memory>

export module crude.core.memory_manager;

import crude.core.free_rbt_allocator;
import crude.core.system_allocator;
import crude.core.aligned_allocator;
import crude.core.cxx_allocator_container;

export namespace crude::core
{

class Memory_Manager
{
public:
  void initialize(core::uint32 defaultFreeRBTCapacity) noexcept;
  void deinitialize() noexcept;
public:
  static std::shared_ptr<CXX_Allocator_Container> getDefaultAllocator() noexcept { return getInstance().getDefaultFreeRBTAllocator(); }
  static std::shared_ptr<Free_RBT_Allocator> getDefaultFreeRBTAllocator() noexcept { return getInstance().m_defaultFreeRBTAllocators; }
  static std::shared_ptr<System_Allocator> getDefaultSystemAllocator() noexcept { return getInstance().m_defaultSystemAllocators; }
  static std::shared_ptr<Aligned_Allocator> getDefaultAlignedAllocator() noexcept { return getInstance().m_defaultAlignedAllocator; }
public:
  static Memory_Manager& getInstance() noexcept;
private:
  Memory_Manager() = default;
  ~Memory_Manager() = default;
private:
  std::shared_ptr<Free_RBT_Allocator>  m_defaultFreeRBTAllocators;
  std::shared_ptr<System_Allocator>    m_defaultSystemAllocators;
  std::shared_ptr<Aligned_Allocator>   m_defaultAlignedAllocator;
};

}