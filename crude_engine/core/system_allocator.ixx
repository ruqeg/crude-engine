module;

export module crude_engine.system_allocator;

import crude_engine.cxx_allocator_container;
import crude_engine.memory_utils;

export namespace crude_engine
{

class System_Allocator : public CXX_Allocator_Container
{
public:
  [[nodiscard]] void* allocate(size_t size) noexcept override;
  void deallocate(void* ptr) noexcept override;
};

}