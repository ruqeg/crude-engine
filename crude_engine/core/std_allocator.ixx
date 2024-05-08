module;

export module crude_engine.std_allocator;

import crude_engine.memory_system;

namespace crude_engine
{

template<class T, class Allocator = Memory_System::Default_Allocator>
struct STD_Allocator
{
  using value_type = T;

  STD_Allocator() = default;

  template<class U>
  constexpr STD_Allocator(const STD_Allocator<U>&) noexcept {}

  [[nodiscard]] T* allocate(size_t n);

  void deallocate(T* ptr, size_t n) noexcept;
};


template<class T, class A1, class U, class A2>
bool operator==(const STD_Allocator<T, A1>&, const STD_Allocator<U, A2>&);

template<class T, class A1, class U, class A2>
bool operator!=(const STD_Allocator<T, A1>&, const STD_Allocator<U, A2>&);

}