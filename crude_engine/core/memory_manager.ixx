module;

export module crude.core.memory_manager;

import crude.core.free_rbt_allocator;
import crude.core.system_allocator;

export namespace crude::core
{

class Memory_Manager
{
public:
  Free_RBT_Allocator& getDefaultFreeRBTAllocators() noexcept;
  System_Allocator& getDefaultSystemAllocators() noexcept;

public:
  static Memory_Manager& getInstance() noexcept;

private:
  Memory_Manager();
  ~Memory_Manager() = default;
  
private:
  static constexpr size_t FREE_RBT_ALLOCATOR_CAPCITY = 10000000u;

private:
  Free_RBT_Allocator  m_defaultFreeRBTAllocators;
  System_Allocator    m_defaultSystemAllocators;
};

}  // namespace crude