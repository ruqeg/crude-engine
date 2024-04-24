#include <core/std_allocator.hpp>

namespace crude_engine
{

template<class T, class A1, class U, class A2>
bool operator==(const STD_Allocator<T, A1>&, const STD_Allocator<U, A2>&) 
{ 
  return true; 
}

template<class T, class A1, class U, class A2>
bool operator!=(const STD_Allocator<T, A1>&, const STD_Allocator<U, A2>&) 
{
  return false; 
}

template<class T, class Allocator>
CRUDE_NODISCARD T* STD_Allocator<T, Allocator>::allocate(std::size_t n)
{
  T* ptr = static_cast<T*>(std::malloc(n * sizeof(T)));
  return ptr;
}

template<class T, class Allocator>
void STD_Allocator<T, Allocator>::deallocate(T* ptr, std::size_t n) noexcept
{
  std::free(ptr);
}

}