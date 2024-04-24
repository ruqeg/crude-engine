#pragma once

#include <core/cxx_allocator_container.hpp>
#include <core/memory_utils.hpp>

namespace crude_engine
{

class System_Allocator : public CXX_Allocator_Container
{
public:
  CRUDE_NODISCARD void* allocate(size_t size) noexcept override;
  void deallocate(void* ptr) noexcept override;
};

}