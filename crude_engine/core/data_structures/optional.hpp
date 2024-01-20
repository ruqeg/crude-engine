#pragma once

#include <core/utility.hpp>

namespace crude_engine
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
  Optional() noexcept;
  ~Optional();
  Optional(Nullopt) noexcept;
  Optional(const T& value) noexcept;
  Optional(const Optional& other) noexcept;
  Optional(Optional&& other) noexcept;

  CRUDE_INLINE Optional& operator=(Nullopt) noexcept;
  CRUDE_INLINE Optional& operator=(const Optional& other) noexcept;
  CRUDE_INLINE Optional& operator=(Optional&& other) noexcept;

  CRUDE_INLINE bool operator==(const Optional& other) const noexcept;
  CRUDE_INLINE bool operator!=(const Optional& other) const noexcept;

  CRUDE_INLINE bool hasValue() const noexcept;

  CRUDE_INLINE T& value() noexcept;
  CRUDE_INLINE const T& value() const noexcept;

  template<class U>
  CRUDE_INLINE T valueOr(U&& u) const noexcept;

  CRUDE_INLINE T& operator*() noexcept;
  CRUDE_INLINE T& operator->() noexcept;

  CRUDE_INLINE const T& operator*() const noexcept;
  CRUDE_INLINE const T& operator->() const noexcept;

  CRUDE_INLINE void reset() noexcept;

  explicit operator bool() const noexcept;

protected:
  union { T m_value; byte m_dummy; };
  bool8  m_engaged;
};

} // namespace crude_engine

#include <core/data_structures/optional.inl>