module;

export module crude.core.iallocator;

export import crude.core.alias;

export namespace crude::core
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