#pragma once

#include <core/alias.hpp>

namespace crude_engine
{

template<class T>
struct Remove_Reference
{
  using Type = T;
};

template<class T>
struct Remove_Reference<T&>
{
  using Type = T;
};

template<class T>
struct Remove_Reference<T&&>
{
  using Type = T;
};

template<class T>
CRUDE_INLINE typename Remove_Reference<T>::Type&& move(T&& x) noexcept
{
  return static_cast<typename Remove_Reference<T>::Type&&>(x);
}

template<class T>
CRUDE_INLINE void swap(T& x, T& y) noexcept
{
  T tmp = move(x);
  x = move(y);
  y = move(tmp);
}


}