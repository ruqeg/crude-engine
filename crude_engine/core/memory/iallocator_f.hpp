#pragma once

#include <core/alias.hpp>

namespace crude_engine
{

class IAllocator_F
{
public:
  virtual void free(void* ptr) noexcept = 0u;
};

}