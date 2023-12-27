#pragma once

#include <core/memory/iallocator_a.hpp>
#include <core/memory/iallocator_f.hpp>

namespace crude_engine
{

class C_Allocator : public IAllocator_A, public IAllocator_F
{
public:
  CRUDE_NODISCARD void* allocate(std::size_t size) noexcept override;
  void free(void* ptr) noexcept override;
};

}