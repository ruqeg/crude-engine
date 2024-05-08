module;

#include <iterator>

export module crude_engine.core.tree_iterator_base.decl;

import crude_engine.core.alias;

export namespace crude_engine
{

template <class Concrete_Iterator, class Node, class Node_Interface>
class Tree_Iterator_Base
{
public:
  using iterator_category = std::input_iterator_tag;
  using difference_type = ptrdiff_t;
  using value_type = Node;
  using pointer = Node*;
  using reference = Node&;

  using Iterator_Category = iterator_category;
  using Difference_Type = difference_type;
  using Value_Type = value_type;
  using Pointer = pointer;
  using Reference = reference;
  using Const_Pointer = const pointer;
  using Const_Reference = const reference;

  Tree_Iterator_Base();
  Tree_Iterator_Base(Node* node);
  Tree_Iterator_Base(const Concrete_Iterator& other);

  inline Concrete_Iterator& operator=(const Concrete_Iterator& other);
  inline Concrete_Iterator& operator=(Concrete_Iterator&& other);
  inline bool operator==(const Concrete_Iterator& other) const;
  inline bool operator!=(const Concrete_Iterator& other) const;

  inline Concrete_Iterator& operator++();
  inline Concrete_Iterator operator++(int);
  inline Concrete_Iterator& operator+=(std::size_t steps);
  inline Concrete_Iterator operator+(std::size_t steps) const;

  inline Concrete_Iterator& operator--();
  inline Concrete_Iterator operator--(int);
  inline Concrete_Iterator& operator-=(std::size_t steps);
  inline Concrete_Iterator operator-(std::size_t steps) const;

  inline Const_Reference operator*() const;
  inline Const_Pointer operator->() const;

protected:
  inline void stepForward();
  inline void stepBack();

  Pointer m_node;

  using My_Type = Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>;
};

} // namespace crude_engine