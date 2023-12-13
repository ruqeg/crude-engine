#pragma once

#include <core/alias.hpp>
#include <iterator>

namespace crude_engine
{

template <class Concrete_Iterator, class Node, class Node_Interface>
class Tree_Iterator_Base
{
public:
  using Difference_Type    = std::size_t;
  using Value_Type         = Node;
  using Reference          = Node&;
  using Pointer            = Node*;
  using Iterator_Category  = std::input_iterator_tag;

  Tree_Iterator_Base();
  Tree_Iterator_Base(Node* node);
  Tree_Iterator_Base(const Concrete_Iterator& other);

  CRUDE_INLINE inline Concrete_Iterator& operator=(const Concrete_Iterator& other);
  CRUDE_INLINE inline Concrete_Iterator& operator=(Concrete_Iterator&& other);
  CRUDE_INLINE inline bool operator==(const Concrete_Iterator& other) const;
  CRUDE_INLINE inline bool operator!=(const Concrete_Iterator& other) const;

  CRUDE_INLINE inline Concrete_Iterator& operator++();
  CRUDE_INLINE inline Concrete_Iterator operator++(int);
  CRUDE_INLINE inline Concrete_Iterator& operator+=(std::size_t steps);
  CRUDE_INLINE inline Concrete_Iterator operator+(std::size_t steps) const;

  CRUDE_INLINE inline Concrete_Iterator& operator--();
  CRUDE_INLINE inline Concrete_Iterator operator--(int);
  CRUDE_INLINE inline Concrete_Iterator& operator-=(std::size_t steps);
  CRUDE_INLINE inline Concrete_Iterator operator-(std::size_t steps) const;

  CRUDE_INLINE inline Reference operator*() const;
  CRUDE_INLINE inline Pointer operator->() const;

protected:
  CRUDE_INLINE inline void stepForward();
  CRUDE_INLINE inline void stepBack();

  Node* m_node;

  using My_Type = Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>;
};

} // namespace crude_engine

#include "tree_iterator_base.inl"