#include <core/data_structures/optional.hpp>

namespace crude_engine
{

template<class T>
Optional<T>::Optional() noexcept
{}

template<class T>
Optional<T>::~Optional()
{
  reset();
}

template<class T>
Optional<T>::Optional(Nullopt) noexcept
{}

template<class T>
Optional<T>::Optional(Const_Reference value) noexcept
{
  construct(value);
}

template<class T>
Optional<T>::Optional(Value_Type&& value) noexcept
{
  construct(Utility::move(value));
}

template<class T>
Optional<T>::Optional(Optional const& other) noexcept
{
  if (other.hasValue())
  {
    construct(other.value());
  }
}

template<class T>
Optional<T>::Optional(Optional&& other) noexcept
{
  if (other.hasValue())
  {
    construct(Utility::move(other.value()));
  }
  other.m_engaged = false;
}

template<class T>
Optional<T>& Optional<T>::operator=(Nullopt) noexcept
{
  reset();
  return*this;
}

template<class T>
Optional<T>& Optional<T>::operator=(Const_Reference value) noexcept
{
  if (hasValue())
  {
    this->value() = value;
  }
  else
  {
    construct(value);
  }

  return *this;
}

template<class T>
Optional<T>& Optional<T>::operator=(Value_Type&& value) noexcept
{
  if (hasValue())
  {
    this->value() = Utility::move(value);
  }
  else
  {
    construct(Utility::move(value));
  }

  return*this;
}

template<class T>
Optional<T>& Optional<T>::operator=(Optional<T> const& other) noexcept
{
  if (other.hasValue())
  {
    if (hasValue())
    {
      this->value() = other.value();
    }
    else
    {
      construct(other.value());
    }
  }
  else
  {
    reset();
  }

  return*this;
}

template<class T>
Optional<T>& Optional<T>::operator=(Optional<T>&& other) noexcept
{
  if (other.hasValue())
  {
    if (hasValue())
    {
      this->value() = Utility::move(other.value());
    }
    else
    {
      construct(Utility::move(other.value()));
    }
  }
  else
  {
    reset();
  }

  return*this;
}

template<class T>
bool Optional<T>::operator==(Optional const& other) const noexcept
{
  return (m_engaged == other.m_engaged) && (m_storage == other.m_storage);
}

template<class T>
bool Optional<T>::operator!=(Optional const& other) const noexcept
{
  return (m_engaged != other.m_engaged) || (m_storage != other.m_storage);
}

template<class T>
bool Optional<T>::hasValue() const noexcept
{
  return m_engaged;
}

template<class T>
Optional<T>::Reference Optional<T>::value() noexcept
{
  CRUDE_ASSERT(hasValue());
  return static_cast<T&>(*getStorage());
}

template<class T>
Optional<T>::Const_Reference Optional<T>::value() const noexcept
{
  CRUDE_ASSERT(hasValue());
  return static_cast<const T&>(*getStorage());
}

template<class T>
Optional<T>::Reference Optional<T>::operator*() noexcept
{
  CRUDE_ASSERT(hasValue());
  return value();
}

template<class T>
Optional<T>::Reference Optional<T>::operator->() noexcept
{
  CRUDE_ASSERT(hasValue());
  return value();
}

template<class T>
Optional<T>::Const_Reference Optional<T>::operator*() const noexcept
{
  CRUDE_ASSERT(hasValue());
  return value();
}

template<class T>
Optional<T>::Const_Reference Optional<T>::operator->() const noexcept
{
  CRUDE_ASSERT(hasValue());
  return value();
}

template<class T>
void Optional<T>::reset() noexcept
{
  if constexpr (!std::is_trivially_destructible<Value_Type>{})
  {
    if (hasValue())
    {
      destruct();
    }
  }
}

template<class T>
Optional<T>::operator bool() const noexcept
{
  return m_engaged;
}

template<class T>
template<class U>
Optional<T>::Value_Type Optional<T>::valueOr(U&& u) const noexcept
{
  return m_engaged ? value() : u;
}

template<class T>
Optional<T>::Pointer Optional<T>::getStorage() noexcept
{
  return reinterpret_cast<Pointer>(&m_storage);
}

template<class T>
Optional<T>::Const_Pointer Optional<T>::getStorage() const noexcept
{
  return reinterpret_cast<Const_Pointer>(&m_storage);
}

template<class T>
template<typename... Args>
void Optional<T>::construct(Args&&... args) noexcept
{
  CRUDE_ASSERT(!hasValue());
  Utility::constructAt<Value_Type>(getStorage(), std::forward<Args>(args)...);
  m_engaged = true;
}

template<class T>
void Optional<T>::destruct() noexcept
{
  if (hasValue())
  {
    Utility::destructorAt<Value_Type>(getStorage());
    m_engaged = false;
  }
}

}