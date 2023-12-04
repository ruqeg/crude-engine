#pragma once

#include <core/alias.hpp>

namespace crude_engine
{

class Memory_Utils
{
public:
  CRUDE_NODISCARD static void* allocate(std::size_t size) noexcept;
  static void free(void* memoryPtr) noexcept;
};

}