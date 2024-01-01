#include <core/utility.hpp>

namespace crude_engine
{

template<class T>
typename Remove_Reference<T>::Type&& Utility::move(T&& x) noexcept
{
  return static_cast<typename Remove_Reference<T>::Type&&>(x);
}

template<class T>
void Utility::swap(T& x, T& y) noexcept
{
  T tmp = Utility::move(x);
  x = Utility::move(y);
  y = Utility::move(tmp);
}

template <class T>
T&& Utility::forward(typename Remove_Reference<T>::Type& x) noexcept
{
  return static_cast<T&&>(x);
}

template <class T>
T&& Utility::forward(typename Remove_Reference<T>::Type&& x) noexcept
{
  return static_cast<T&&>(x);
}

}