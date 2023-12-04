#pragma once

#include <core/alias.hpp>

namespace crude_engine
{

class C_Allocator
{
public:
  static CRUDE_NODISCARD void* allocate(std::size_t size) noexcept;
  static void free(void* ptr) noexcept;
};

}