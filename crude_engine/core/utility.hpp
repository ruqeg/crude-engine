#pragma once

#include <core/alias.hpp>

namespace crude_engine
{

template<class T, T v>
struct Integral_Constant
{
  static constexpr T value = v;

  using Value_Type = T;
  using Type = Integral_Constant;
  
  constexpr operator Value_Type() const noexcept { return value; }
  constexpr Value_Type operator()() const noexcept { return value; }
};

template<bool B>
using Bool_Constant = Integral_Constant<bool, B>;

using True_Type = Bool_Constant<true>;
using False_Type = Bool_Constant<false>;

template<class T, class U>
struct Is_Same : False_Type {};

template<class T>
struct Is_Same<T, T> : True_Type {};

template< class T, class U >
constexpr bool Is_Same_V = Is_Same<T, U>::value;

template<class T, class U>
concept Same_As = Is_Same_V<T, U> && Is_Same_V<U, T>;

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

template<std::size_t Len, std::size_t Align>
struct Aligned_Storage
{
  struct Type
  {
    alignas(Align) uint8 data[Len];
  };
};

template< std::size_t Len, std::size_t Align>
using Aligned_Storage_T = typename Aligned_Storage<Len, Align>::Type;

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