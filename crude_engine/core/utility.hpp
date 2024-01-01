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

class Utility
{
public:
  template<class T>
  static CRUDE_INLINE typename Remove_Reference<T>::Type&& move(T&& x) noexcept;

  template<class T>
  static CRUDE_INLINE void swap(T& x, T& y) noexcept;

  template <class T>
  static CRUDE_INLINE T&& forward(typename Remove_Reference<T>::Type& x) noexcept;

  template <class T>
  static CRUDE_INLINE T&& forward(typename Remove_Reference<T>::Type&& x) noexcept;
};

} // namespace crude_engine

#include <core/utility.inl>