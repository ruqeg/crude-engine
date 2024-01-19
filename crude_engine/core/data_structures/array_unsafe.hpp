#pragma once

#include <core/debug/assert.hpp>
#include <core/utility.hpp>
#include <core/algorithms.hpp>
#include <core/data_structures/iterator_base.hpp>

namespace crude_engine
{

template<class T>
class Array_Unsafe
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
      Contiguous_Iterator_Base<Iterator, Value_Type>(other.m_ptr)
    {};
    Iterator() noexcept
      :
      Contiguous_Iterator_Base<Iterator, Value_Type>()
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
  Array_Unsafe();
  explicit Array_Unsafe(Pointer data, Size_Type size);
  ~Array_Unsafe() = default;
  Array_Unsafe(const Array_Unsafe& other);
  Array_Unsafe(Array_Unsafe&& other);
  Array_Unsafe& operator=(const Array_Unsafe& other);
  Array_Unsafe& operator=(Array_Unsafe&& other);

public:
  CRUDE_INLINE Pointer data() noexcept;
  CRUDE_INLINE Const_Pointer data() const noexcept;

  CRUDE_INLINE Reference get(std::size_t index) noexcept;
  CRUDE_INLINE Const_Reference get(std::size_t index) const noexcept;

  void set(std::size_t index, Const_Reference value) noexcept;

  CRUDE_INLINE Reference operator[](std::size_t index) noexcept;
  CRUDE_INLINE Const_Reference operator[](std::size_t index) const noexcept;

  Size_Type size() const noexcept;
  void fill(Const_Reference value) noexcept;

  CRUDE_INLINE Reference front() noexcept;
  CRUDE_INLINE Const_Reference front() const noexcept;

  CRUDE_INLINE Reference back() noexcept;
  CRUDE_INLINE Const_Reference back() const noexcept;

  CRUDE_INLINE Const_Iterator cbegin() const noexcept;
  CRUDE_INLINE Const_Iterator cend() const noexcept;

  CRUDE_INLINE Const_Iterator begin() const noexcept;
  CRUDE_INLINE Iterator begin() noexcept;

  CRUDE_INLINE Const_Iterator end() const noexcept;
  CRUDE_INLINE Iterator end() noexcept;

  CRUDE_INLINE Const_Iterator crbegin() const noexcept;
  CRUDE_INLINE Const_Iterator crend() const noexcept;

  CRUDE_INLINE Const_Iterator rbegin() const noexcept;
  CRUDE_INLINE Iterator rbegin() noexcept;

  CRUDE_INLINE Const_Iterator rend() const noexcept;
  CRUDE_INLINE Iterator rend() noexcept;

protected:
  Pointer    m_data;
  Size_Type  m_size;
};

} // namespace crude_engine

#include <core/data_structures/array_unsafe.inl>