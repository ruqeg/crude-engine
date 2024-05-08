module;

export module crude_engine.iallocator;

import crude_engine.alias;

export namespace crude_engine
{

class IAllocator_Allocate
{
public:
  virtual [[nodiscard]] void* allocate(size_t size) noexcept = 0u;
};


class IAllocator_Deallocate
{
public:
  virtual void deallocate(void* ptr) noexcept = 0u;
};

class IAllocator : public IAllocator_Allocate, public IAllocator_Deallocate
{};

}