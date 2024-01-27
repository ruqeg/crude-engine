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
  using Value_Type       = T;
  using Size_Type        = std::size_t;
  using Difference_Type  = std::ptrdiff_t;
  using Reference        = Value_Type&;
  using Const_Reference  = Value_Type const&;
  using Pointer          = Value_Type*;
  using Const_Pointer    = Value_Type const*;

public:
  Optional() noexcept;
  ~Optional();
  Optional(Nullopt) noexcept;
  Optional(Const_Reference value) noexcept;
  Optional(Value_Type&& value) noexcept;
  Optional(Optional const& other) noexcept;
  Optional(Optional&& other) noexcept;

  CRUDE_INLINE Optional& operator=(Nullopt) noexcept;
  CRUDE_INLINE Optional& operator=(Const_Reference value) noexcept;
  CRUDE_INLINE Optional& operator=(Value_Type&& value) noexcept;
  CRUDE_INLINE Optional& operator=(Optional const& other) noexcept;
  CRUDE_INLINE Optional& operator=(Optional&& other) noexcept;

  CRUDE_INLINE bool operator==(Optional const& other) const noexcept;
  CRUDE_INLINE bool operator!=(Optional const& other) const noexcept;

  CRUDE_INLINE bool hasValue() const noexcept;

  CRUDE_INLINE Reference value() noexcept;
  CRUDE_INLINE Const_Reference value() const noexcept;

  template<class U>
  CRUDE_INLINE Value_Type valueOr(U&& u) const noexcept;

  CRUDE_INLINE Reference operator*() noexcept;
  CRUDE_INLINE Reference operator->() noexcept;

  CRUDE_INLINE Const_Reference operator*() const noexcept;
  CRUDE_INLINE Const_Reference operator->() const noexcept;

  CRUDE_INLINE void reset() noexcept;

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

} // namespace crude_engine

#include <core/data_structures/optional.inl>