module crude_engine.utility;

namespace crude_engine
{

template<class T>
typename Remove_Reference<T>::Type&& move(T&& x) noexcept
{
  return static_cast<typename Remove_Reference<T>::Type&&>(x);
}

template<class T>
void swap(T& x, T& y) noexcept
{
  T tmp = move(x);
  x = move(y);
  y = move(tmp);
}

template <class T>
T&& forward(typename Remove_Reference<T>::Type& x) noexcept
{
  return static_cast<T&&>(x);
}

template <class T>
T&& forward(typename Remove_Reference<T>::Type&& x) noexcept
{
  return static_cast<T&&>(x);
}

template<class T, class... Args >
T* constructAt(T* ptr, Args&&... args) noexcept
{
  return ::new (ptr) T(forward<Args>(args)...);
}

template<class T>
void destructorAt(T* ptr) noexcept
{
  ptr->~T();
}

}