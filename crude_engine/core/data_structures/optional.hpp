#pragma once

#include <core/utility.hpp>
#include <core/memory/memory_utils.hpp>

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
  using Value_Type = T;

public:
  Optional() noexcept;
  ~Optional();
  Optional(Nullopt) noexcept;
  Optional(const T& value) noexcept;
  Optional(T&& value) noexcept;
  Optional(const Optional& other) noexcept;
  Optional(Optional&& other) noexcept;

  CRUDE_INLINE Optional& operator=(Nullopt) noexcept;
  CRUDE_INLINE Optional& operator=(const T& value) noexcept;
  CRUDE_INLINE Optional& operator=(T&& value) noexcept;
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
  T* getStorage() noexcept;
  const T* getStorage() const noexcept;

  template<typename... Args>
  void construct(Args&&... args) noexcept;

  void destruct() noexcept;

protected:
  Aligned_Storage_T<sizeof(T), alignof(T)> m_storage;
  bool8  m_engaged = false;
};

} // namespace crude_engine

#include <core/data_structures/optional.inl>