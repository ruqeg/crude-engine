module;

export module crude.core.system_allocator;

import crude.core.cxx_allocator_container;
import crude.core.memory_utils;

export namespace crude::core
{

class System_Allocator : public CXX_Allocator_Container
{
public:
  [[nodiscard]] void* allocate(size_t size) noexcept override;
  void deallocate(void* ptr) noexcept override;
};

}