#pragma once

#include <core/iterator_base.hpp>
#include <core/assert.hpp>

namespace crude_engine
{

template<class Node>
class List_Iterator_Interface_Base
{
public:
  static CRUDE_INLINE void stepForward(Node*& ptr) noexcept;
  static CRUDE_INLINE void stepBack(Node*& ptr) noexcept;
  static CRUDE_INLINE void jumpForward(Node*& ptr, std::size_t steps) noexcept;
  static CRUDE_INLINE void jumpBack(Node*& ptr, std::size_t steps) noexcept;
};

template <class Concrete_Iterator,
          class Node,
          class Interface = List_Iterator_Interface_Base<Node>>
class List_Iterator_Base : public Bidirectional_Iterator_Base<Concrete_Iterator, Node, Interface>
{
public:
  using My_Class = List_Iterator_Base<Concrete_Iterator, Node, Interface>;

  using iterator_category  = std::bidirectional_iterator_tag;
  using difference_type    = std::ptrdiff_t;
  using value_type         = Node;
  using pointer            = Node*;
  using reference          = Node&;

  using Iterator_Category = iterator_category;
  using Difference_Type   = difference_type;
  using Value_Type        = value_type;
  using Pointer           = pointer;
  using Reference         = reference;
  using Const_Pointer     = const pointer;
  using Const_Reference   = const reference;

  List_Iterator_Base() noexcept;
  List_Iterator_Base(Pointer ptr) noexcept;
  List_Iterator_Base(const Concrete_Iterator& other) noexcept;
};

}

#include <core/data_structures/list_iterator_base.inl>