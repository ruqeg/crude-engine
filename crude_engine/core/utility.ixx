module; 

export module crude_engine.core.utility;

export import crude_engine.core.alias;

export namespace crude_engine
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

template<size_t Len, size_t Align>
struct Aligned_Storage
{
  struct Type
  {
    alignas(Align) uint8 data[Len];
  };
};

template<size_t Len, size_t Align>
using Aligned_Storage_T = typename Aligned_Storage<Len, Align>::Type;

// generate unique id for class
template<typename T> 
struct CPP_Type
{
  static size_t id()
  {
    return reinterpret_cast<size_t>(&CPP_Type<T>::id);
  }
};

template<class T>
inline typename Remove_Reference<T>::Type&& move(T&& x) noexcept
{
  return static_cast<typename Remove_Reference<T>::Type&&>(x);
}

template<class T>
inline void swap(T& x, T& y) noexcept
{
  T tmp = move(x);
  x = move(y);
  y = move(tmp);
}

template <class T>
inline T&& forward(typename Remove_Reference<T>::Type& x) noexcept
{
  return static_cast<T&&>(x);
}

template <class T>
inline T&& forward(typename Remove_Reference<T>::Type&& x) noexcept
{
  return static_cast<T&&>(x);
}

template<class T, class... Args >
inline T* constructAt(T* ptr, Args&&... args) noexcept
{
  return ::new (ptr) T(forward<Args>(args)...);
}

template<class T>
inline void destructorAt(T* ptr) noexcept
{
  ptr->~T();
}

} // namespace crude_engine