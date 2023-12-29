#pragma once

#include <core/alias.hpp>
#include <iterator>

namespace crude_engine
{

template<class T>
class Iterator_Interface_Base
{
public:
  static CRUDE_INLINE void stepForward(T*& ptr);
  static CRUDE_INLINE void stepBack(T*& ptr);
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

  Contiguous_Iterator_Base();
  Contiguous_Iterator_Base(Pointer ptr);
  Contiguous_Iterator_Base(const Concrete_Iterator& other);

  CRUDE_INLINE Concrete_Iterator& operator=(const Concrete_Iterator& other);
  CRUDE_INLINE Concrete_Iterator& operator=(Concrete_Iterator&& other);
  CRUDE_INLINE bool operator==(const Concrete_Iterator& other) const;
  CRUDE_INLINE bool operator!=(const Concrete_Iterator& other) const;

  CRUDE_INLINE Concrete_Iterator& operator++();
  CRUDE_INLINE Concrete_Iterator operator++(int);
  CRUDE_INLINE Concrete_Iterator& operator+=(std::size_t steps);
  CRUDE_INLINE Concrete_Iterator operator+(std::size_t steps) const;

  CRUDE_INLINE Concrete_Iterator& operator--();
  CRUDE_INLINE Concrete_Iterator operator--(int);
  CRUDE_INLINE Concrete_Iterator& operator-=(std::size_t steps);
  CRUDE_INLINE Concrete_Iterator operator-(std::size_t steps) const;

  CRUDE_INLINE Reference operator*();
  CRUDE_INLINE Pointer operator->();

  CRUDE_INLINE Const_Reference operator*() const;
  CRUDE_INLINE Const_Pointer operator->() const;

  static_assert(std::contiguous_iterator<Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>>);

protected:
  Pointer m_ptr;
};

} // namespace crude_engine

#include <core/data_structures/iterator_base.inl>