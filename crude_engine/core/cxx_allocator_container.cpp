module crude_engine.core.cxx_allocator_container;

import crude_engine.core.utility;

namespace crude_engine
{

template<class T, typename ...Args>
[[nodiscard]] T* CXX_Allocator_Container::cxxAllocate(Args&& ...args) noexcept
{
  return constructAt(allocate(sizeof(T)), forward<Args>(args)...);
}

template<class T, typename ...Args>
[[nodiscard]] T* CXX_Allocator_Container::cxxAllocate(size_t n, Args && ...args) noexcept
{
  T* ptr = reinterpret_cast<T*>(allocate(sizeof(T) * n));

  for (size_t i = 0; i < n; ++i)
  {
    constructAt(ptr + i, forward<Args>(args)...);
  }
  return ptr;
}

template<class T>
void CXX_Allocator_Container::cxxDeallocate(T* ptr) noexcept
{
  destructorAt(ptr);
  deallocate(reinterpret_cast<void*>(ptr));
}

template<class T>
void CXX_Allocator_Container::cxxDeallocate(size_t n, T* ptr) noexcept
{
  for (size_t i = 0; i < n; ++i)
  {
    destructorAt(ptr + i);
  }
  deallocate(reinterpret_cast<void*>(ptr));
}

}