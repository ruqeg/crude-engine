#pragma once

#include <core/memory_system.hpp>

namespace crude_engine
{

template<class T, class Allocator = Memory_System::Default_Allocator>
struct STD_Allocator
{
  using value_type = T;

  STD_Allocator() = default;

  template<class U>
  constexpr STD_Allocator(const STD_Allocator<U>&) noexcept {}

  CRUDE_NODISCARD T* allocate(std::size_t n);

  void deallocate(T* ptr, std::size_t n) noexcept;
};


template<class T, class A1, class U, class A2>
bool operator==(const STD_Allocator<T, A1>&, const STD_Allocator<U, A2>&);

template<class T, class A1, class U, class A2>
bool operator!=(const STD_Allocator<T, A1>&, const STD_Allocator<U, A2>&);

}

#include <core/std_allocator.inl>