#pragma once

#include <core/alias.hpp>

namespace crude_engine
{

class IAllocator_A
{
public:
  CRUDE_NODISCARD virtual void* allocate(std::size_t size) noexcept = 0u;
};

}