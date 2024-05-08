module;

export module crude_engine.optional;

import crude_engine.utility;

export namespace crude_engine
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

} // namespace crude_engine