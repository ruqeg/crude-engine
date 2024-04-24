#pragma once

#include <core/alias.hpp>

namespace crude_engine
{

class IAllocator_Allocate
{
public:
  virtual CRUDE_NODISCARD void* allocate(size_t size) noexcept = 0u;
};


class IAllocator_Deallocate
{
public:
  virtual void deallocate(void* ptr) noexcept = 0u;
};

class IAllocator : public IAllocator_Allocate, public IAllocator_Deallocate
{};

}