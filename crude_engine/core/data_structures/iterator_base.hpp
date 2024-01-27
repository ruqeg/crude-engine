#pragma once

#include <core/alias.hpp>
#include <iterator>

namespace crude_engine
{

template<class T>
class Iterator_Interface_Base
{
public:
  static CRUDE_INLINE void stepForward(T*& ptr) noexcept;
  static CRUDE_INLINE void stepBack(T*& ptr) noexcept;
  static CRUDE_INLINE void jumpForward(T*& ptr, std::size_t steps) noexcept;
  static CRUDE_INLINE void jumpBack(T*& ptr, std::size_t steps) noexcept;
};

template <class Concrete_Iterator, 
          class Type, 
          class Interface = Iterator_Interface_Base<Type>>
class Contiguous_Iterator_Base
{
public:
  using My_Class = Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>;

  using iterator_category  = std::contiguous_iterator_tag;
  using difference_type    = std::ptrdiff_t;
  using value_type         = Type;
  using pointer            = Type*;
  using reference          = Type&;

  using Iterator_Category = iterator_category;
  using Difference_Type   = difference_type;
  using Value_Type        = value_type;
  using Pointer           = pointer;
  using Reference         = reference;
  using Const_Pointer     = const pointer;
  using Const_Reference   = const reference;

  Contiguous_Iterator_Base() noexcept;
  Contiguous_Iterator_Base(Pointer ptr) noexcept;
  Contiguous_Iterator_Base(const Concrete_Iterator& other) noexcept;

  CRUDE_INLINE Concrete_Iterator& operator=(const Concrete_Iterator& other) noexcept;
  CRUDE_INLINE Concrete_Iterator& operator=(Concrete_Iterator&& other) noexcept;
  CRUDE_INLINE bool operator==(const Concrete_Iterator& other) const noexcept;
  CRUDE_INLINE bool operator!=(const Concrete_Iterator& other) const noexcept;

  CRUDE_INLINE Concrete_Iterator& operator++() noexcept;
  CRUDE_INLINE Concrete_Iterator operator++(int) noexcept;
  CRUDE_INLINE Concrete_Iterator& operator+=(std::size_t steps) noexcept;
  CRUDE_INLINE Concrete_Iterator operator+(std::size_t steps) const noexcept;

  CRUDE_INLINE Concrete_Iterator& operator--() noexcept;
  CRUDE_INLINE Concrete_Iterator operator--(int) noexcept;
  CRUDE_INLINE Concrete_Iterator& operator-=(std::size_t steps) noexcept;
  CRUDE_INLINE Concrete_Iterator operator-(std::size_t steps) const noexcept;

  CRUDE_INLINE Reference operator*() noexcept;
  CRUDE_INLINE Pointer operator->() noexcept;

  CRUDE_INLINE Const_Reference operator*() const noexcept;
  CRUDE_INLINE Const_Pointer operator->() const noexcept;

  // !TODO
  //CRUDE_STATIC_ASSERT(std::contiguous_iterator<Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>>);

protected:
  Pointer m_ptr;
};

} // namespace crude_engine

#include <core/data_structures/iterator_base.inl>