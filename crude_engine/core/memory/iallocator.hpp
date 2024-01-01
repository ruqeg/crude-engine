#pragma once

#include <core/alias.hpp>

namespace crude_engine
{

class IAllocator_Alloc
{
public:
  virtual CRUDE_NODISCARD void* allocate(std::size_t size) noexcept = 0u;
};


class IAllocator_Free
{
public:
  virtual void free(void* ptr) noexcept = 0u;
};

class IAllocator : public IAllocator_Alloc, public IAllocator_Free
{};

}