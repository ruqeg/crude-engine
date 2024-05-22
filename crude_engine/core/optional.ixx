module;

#include <utility>
#include <type_traits>

export module crude.core.optional;

import crude.core.assert;
import crude.core.utility;

export namespace crude::core
{

struct Nullopt
{
  constexpr explicit Nullopt(int) {}
};

constexpr Nullopt nullopt(1);

template<class T>
class Optional
{
public:
  using Value_Type = T;
  using Reference = Value_Type&;
  using Const_Reference = Value_Type const&;
  using Pointer = Value_Type*;
  using Const_Pointer = Value_Type const*;

public:
  Optional() noexcept;
  ~Optional();
  Optional(Nullopt) noexcept;
  Optional(Const_Reference value) noexcept;
  Optional(Value_Type&& value) noexcept;
  Optional(Optional const& other) noexcept;
  Optional(Optional&& other) noexcept;

  inline Optional& operator=(Nullopt) noexcept;
  inline Optional& operator=(Const_Reference value) noexcept;
  inline Optional& operator=(Value_Type&& value) noexcept;
  inline Optional& operator=(Optional const& other) noexcept;
  inline Optional& operator=(Optional&& other) noexcept;

  inline bool operator==(Optional const& other) const noexcept;
  inline bool operator!=(Optional const& other) const noexcept;

  inline bool hasValue() const noexcept;

  inline Reference value() noexcept;
  inline Const_Reference value() const noexcept;

  template<class U>
  inline Value_Type valueOr(U&& u) const noexcept;

  inline Reference operator*() noexcept;
  inline Reference operator->() noexcept;

  inline Const_Reference operator*() const noexcept;
  inline Const_Reference operator->() const noexcept;

  inline void reset() noexcept;

  explicit operator bool() const noexcept;

protected:
  Pointer getStorage() noexcept;
  Const_Pointer getStorage() const noexcept;

  template<typename... Args>
  void construct(Args&&... args) noexcept;

  void destruct() noexcept;

protected:
  Aligned_Storage_T<sizeof(T), alignof(T)> m_storage;
  bool8  m_engaged = false;
};


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
  construct(std::move(value));
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
    construct(std::move(other.value()));
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
    this->value() = std::move(value);
  }
  else
  {
    construct(std::move(value));
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
      this->value() = std::move(other.value());
    }
    else
    {
      construct(std::move(other.value()));
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
  assert(hasValue());
  return static_cast<T&>(*getStorage());
}

template<class T>
Optional<T>::Const_Reference Optional<T>::value() const noexcept
{
  assert(hasValue());
  return static_cast<const T&>(*getStorage());
}

template<class T>
Optional<T>::Reference Optional<T>::operator*() noexcept
{
  assert(hasValue());
  return value();
}

template<class T>
Optional<T>::Reference Optional<T>::operator->() noexcept
{
  assert(hasValue());
  return value();
}

template<class T>
Optional<T>::Const_Reference Optional<T>::operator*() const noexcept
{
  assert(hasValue());
  return value();
}

template<class T>
Optional<T>::Const_Reference Optional<T>::operator->() const noexcept
{
  assert(hasValue());
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
  assert(!hasValue());
  constructAt<Value_Type>(getStorage(), std::forward<Args>(args)...);
  m_engaged = true;
}

template<class T>
void Optional<T>::destruct() noexcept
{
  if (hasValue())
  {
    destructorAt<Value_Type>(getStorage());
    m_engaged = false;
  }
}

} // namespace crude