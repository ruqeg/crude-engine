#pragma once

#include <core/debug/assert.hpp>
#include <core/data_structures/iterator_base.hpp>

namespace crude_engine
{

template<class T, std::size_t N>
class Array_Stack
{
public:
  using Value_Type       = T;
  using Size_Type        = std::size_t;
  using Difference_Type  = std::ptrdiff_t;
  using Reference        = Value_Type&;
  using Const_Reference  = const Value_Type&;
  using Pointer          = Value_Type*;
  using Const_Pointer    = const Value_Type*;

public:
  class Iterator : public Contiguous_Iterator_Base<Iterator, Value_Type>
  {
  public:
    using Contiguous_Iterator_Base<Iterator, Value_Type>::Contiguous_Iterator_Base;

    Iterator(const Iterator& other) noexcept
      : 
      Tree_Iterator_Base<Iterator, Value_Type>(other.m_ptr)
    {};
    Iterator() noexcept
      : 
      Tree_Iterator_Base<Iterator, Value_Type>()
    {};

    Iterator& operator=(const Iterator&) noexcept = default;

  private:
    friend class Const_Iterator;
  };

  class Const_Iterator : public Contiguous_Iterator_Base<Const_Iterator, const Value_Type>
  {
  public:
    using Contiguous_Iterator_Base<Const_Iterator, const Value_Type>::Contiguous_Iterator_Base;
    Const_Iterator(const Const_Iterator& other) noexcept
      : 
      Contiguous_Iterator_Base<Const_Iterator, const Value_Type>(other.m_ptr)
    {};
    Const_Iterator(const Iterator& other) noexcept
      :
      Contiguous_Iterator_Base<Const_Iterator, const Value_Type>(other.m_ptr) {};
    Const_Iterator() noexcept
      :
      Contiguous_Iterator_Base<Const_Iterator, const Value_Type>()
    {};

    Const_Iterator& operator=(const Const_Iterator&) noexcept = default;
  };

public:
  Array_Stack() = default;
  ~Array_Stack() = default;
  template <typename... Args>
  Array_Stack(Args&&... args);
  Array_Stack(const Array_Stack& other);
  Array_Stack(Array_Stack&&) = delete;
  Array_Stack& operator=(const Array_Stack& other);
  Array_Stack& operator=(Array_Stack&&) = delete;

public:
  Pointer data() noexcept;
  Const_Pointer data() const noexcept;

  Reference get(std::size_t index) noexcept;
  Const_Reference get(std::size_t index) const noexcept;

  void set(std::size_t index, Const_Reference value) noexcept;

  Reference operator[](std::size_t index) noexcept;
  Const_Reference operator[](std::size_t index) const noexcept;

  Size_Type size() const noexcept;
  void fill(Const_Reference value) noexcept;

  Reference front() noexcept;
  Const_Reference front() const noexcept;

  Reference back() noexcept;
  Const_Reference back() const noexcept;

  Const_Iterator cbegin() const noexcept;
  Const_Iterator cend() const noexcept;

  Const_Iterator begin() const noexcept;
  Iterator begin() noexcept;

  Const_Iterator end() const noexcept;
  Iterator end() noexcept;

  Const_Iterator crbegin() const noexcept;
  Const_Iterator crend() const noexcept;

  Const_Iterator rbegin() const noexcept;
  Iterator rbegin() noexcept;

  Const_Iterator rend() const noexcept;
  Iterator rend() noexcept;

  static_assert(N > 0);

protected:
  Value_Type m_data[N];
};

} // namespace crude_engine

#include <core/data_structures/array_stack.inl>