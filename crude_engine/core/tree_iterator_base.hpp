#pragma once

#include <core/alias.hpp>
#include <iterator>

namespace crude_engine
{

template <class Concrete_Iterator, class Node, class Node_Interface>
class Tree_Iterator_Base
{
public:
  using iterator_category = std::input_iterator_tag;
  using difference_type   = std::ptrdiff_t;
  using value_type        = Node;
  using pointer           = Node*;
  using reference         = Node&;

  using Iterator_Category  = iterator_category;
  using Difference_Type    = difference_type;
  using Value_Type         = value_type;
  using Pointer            = pointer;
  using Reference          = reference;
  using Const_Pointer      = const pointer;
  using Const_Reference    = const reference;

  Tree_Iterator_Base();
  Tree_Iterator_Base(Node* node);
  Tree_Iterator_Base(const Concrete_Iterator& other);

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

  CRUDE_INLINE Const_Reference operator*() const;
  CRUDE_INLINE Const_Pointer operator->() const;

protected:
  CRUDE_INLINE void stepForward();
  CRUDE_INLINE void stepBack();

  Pointer m_node;

  using My_Type = Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>;
};

} // namespace crude_engine

#include <core/tree_iterator_base.inl>