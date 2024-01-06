#pragma once

#include <core/utility.hpp>

namespace crude_engine
{

struct Nullopt 
{
  constexpr explicit Nullopt(int) {}
};

template<class T>
class Optional
{
public:
  Optional() noexcept;
  ~Optional();
  template<typename... Args>
  Optional(Args&&... args) noexcept;
  Optional(const T& value) noexcept;
  Optional(const Optional& other) noexcept;
  Optional(Optional&& other) noexcept;

  CRUDE_INLINE Optional& operator=(Nullopt) noexcept;
  CRUDE_INLINE Optional& operator=(const Optional& other) noexcept;
  CRUDE_INLINE Optional& operator=(Optional&& other) noexcept;

  CRUDE_INLINE bool operator==(const Optional& other) const noexcept;
  CRUDE_INLINE bool operator!=(const Optional& other) const noexcept;

  CRUDE_INLINE bool hasValue() const noexcept;

  CRUDE_INLINE T& getValue() noexcept;
  CRUDE_INLINE const T& getValue() const noexcept;

  template<class U>
  CRUDE_INLINE T valueOr(U&& u) const noexcept;

  CRUDE_INLINE T& operator*() noexcept;
  CRUDE_INLINE T& operator->() noexcept;

  CRUDE_INLINE T& operator*() const noexcept;
  CRUDE_INLINE T& operator->() const noexcept;

  CRUDE_INLINE void reset() noexcept;

  explicit operator bool() const noexcept;

protected:
  union { T m_value; byte m_dummy; };
  bool8  m_engaged;
};

template<class T>
Optional<T>::Optional() noexcept
  :
  m_dummy(0u),
  m_engaged(true)
{}

template<class T>
Optional<T>::~Optional()
{
  if constexpr (!std::is_trivially_destructible<T>{})
  {
    if (m_engaged)
    {
      m_value.~T();
    }
  }
}

template<class T>
template<typename ...Args>
Optional<T>::Optional(Args&& ...args) noexcept
  :
  m_value(Utility::forward<Args>(args)...),
  m_engaged(true)
{}

template<class T>
Optional<T>::Optional(const T& value) noexcept
  :
  m_value(value),
  m_engaged(true)
{}

template<class T>
Optional<T>::Optional(const Optional& other) noexcept
{
  m_value = other.m_value;
  m_engaged = other.m_engaged;
}

template<class T>
Optional<T>::Optional(Optional&& other) noexcept
{
  m_value = Utility::move(other.m_value);
  m_engaged = other.m_engaged;

  other.m_engaged = false;
}

template<class T>
Optional<T>& Optional<T>::operator=(const Optional<T>& other) noexcept
{
  m_value = other.m_value;
  m_engaged = other.m_engaged;
}

template<class T>
Optional<T>& Optional<T>::operator=(Optional<T>&& other) noexcept
{
  m_value = Utility::move(other.m_value);
  m_engaged = other.m_engaged;

  other.m_engaged = false;
}

template<class T>
bool Optional<T>::operator==(const Optional& other) const noexcept
{
  return (m_engaged == other.m_engaged) && (m_value == other.m_value);
}

template<class T>
bool Optional<T>::operator!=(const Optional& other) const noexcept
{
  return (m_engaged != other.m_engaged) || (m_value != other.m_value);
}

template<class T>
bool Optional<T>::hasValue() const noexcept
{
  return m_engaged;
}

template<class T>
T& Optional<T>::getValue() noexcept
{
  return m_value;
}

template<class T>
const T& Optional<T>::getValue() const noexcept
{
  return m_value;
}

template<class T>
T& Optional<T>::operator*() noexcept
{
  return m_value;
}

template<class T>
T& Optional<T>::operator->() noexcept
{
  return m_value;
}

template<class T>
T& Optional<T>::operator*() const noexcept
{
  return m_value;
}

template<class T>
T& Optional<T>::operator->() const noexcept
{
  return m_value;
}

template<class T>
void Optional<T>::reset() noexcept
{
  m_engaged = false;
}

template<class T>
Optional<T>::operator bool() const noexcept
{
  return m_engaged;
}

template<class T>
template<class U>
T Optional<T>::valueOr(U&& u) const noexcept
{
  return m_engaged ? m_value : u;
}

} // namespace crude_engine