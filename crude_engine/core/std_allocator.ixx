module;

export module crude.core.std_allocator;

export import crude.core.memory;

export namespace crude::core
{

template<class T>
struct STD_Allocator
{
  using value_type = T;

  STD_Allocator() = default;

  template<class U>
  constexpr STD_Allocator(const STD_Allocator<U>&) noexcept {}

  [[nodiscard]] T* allocate(core::size_t size) noexcept
  {
    return static_cast<T*>(defaultAllocate(size * sizeof(T)));
  }

  void deallocate(T* ptr, core::size_t size) noexcept
  {
    defaultDeallocate(ptr);
  }
};

template<class T, class U>
bool operator==(const STD_Allocator<T>&, const STD_Allocator<U>&) { return true; }

template<class T, class U>
bool operator!=(const STD_Allocator<T>&, const STD_Allocator<U>&) { return false; }

}