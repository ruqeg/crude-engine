module crude_engine.std_allocator;

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
[[nodiscard]] T* STD_Allocator<T, Allocator>::allocate(size_t n)
{
  T* ptr = static_cast<T*>(Allocator::allocate(n * sizeof(T)));
  return ptr;
}

template<class T, class Allocator>
void STD_Allocator<T, Allocator>::deallocate(T* ptr, size_t n) noexcept
{
  Allocator::deallocate(ptr);
}

}