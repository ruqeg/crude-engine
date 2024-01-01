#pragma once

#include <core/memory/iallocator.hpp>

namespace crude_engine
{

class C_Allocator : public IAllocator
{
public:
  CRUDE_NODISCARD void* allocate(std::size_t size) noexcept override;
  void free(void* ptr) noexcept override;
};

}