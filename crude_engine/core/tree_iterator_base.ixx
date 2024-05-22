module;

#include <iterator>

export module crude.core.tree_iterator_base;

import crude.core.alias;

export namespace crude::core
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

template <class Concrete_Iterator, class Node, class Node_Interface>
void Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::stepForward()
{
  if (Node_Interface::getRight(m_node) != nullptr)
  {
    m_node = Node_Interface::getRight(m_node);
    while (Node_Interface::getLeft(m_node) != nullptr)
    {
      m_node = Node_Interface::getLeft(m_node);
    }
  }
  else
  {
    while ((Node_Interface::getParent(m_node) != nullptr) && (Node_Interface::getRight(Node_Interface::getParent(m_node)) == m_node))
    {
      m_node = Node_Interface::getParent(m_node);
    }

    if (Node_Interface::getParent(m_node) == nullptr)
    {
      m_node = nullptr;
    }
    else
    {
      m_node = Node_Interface::getParent(m_node);
    }
  }
}

template <class Concrete_Iterator, class Node, class Node_Interface>
void Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::stepBack()
{
  if (Node_Interface::getLeft(m_node) != nullptr)
  {
    m_node = Node_Interface::getLeft(m_node);
    while (Node_Interface::getRight(m_node) != nullptr)
    {
      m_node = Node_Interface::getRight(m_node);
    }
  }
  else
  {
    while ((Node_Interface::getParent(m_node) != nullptr) && (Node_Interface::getLeft(Node_Interface::getParent(m_node)) == m_node))
    {
      m_node = Node_Interface::getParent(m_node);
    }

    if (Node_Interface::getParent(m_node) == nullptr)
    {
      m_node = nullptr;
    }
    else
    {
      m_node = Node_Interface::getParent(m_node);
    }
  }
}

template <class Concrete_Iterator, class Node, class Node_Interface>
Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::Tree_Iterator_Base()
  :
  m_node(nullptr)
{}

template <class Concrete_Iterator, class Node, class Node_Interface>
Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::Tree_Iterator_Base(Node* node)
  :
  m_node(node)
{}

template <class Concrete_Iterator, class Node, class Node_Interface>
Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::Tree_Iterator_Base(const Concrete_Iterator& other)
  :
  m_node(other.m_node)
{}

template <class Concrete_Iterator, class Node, class Node_Interface>
Concrete_Iterator& Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator=(const Concrete_Iterator& other)
{
  this->m_node = other.m_node;
  return *(static_cast<Concrete_Iterator*>(this));
}

template <class Concrete_Iterator, class Node, class Node_Interface>
Concrete_Iterator& Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator=(Concrete_Iterator&& other)
{
  m_node = other.m_node;
  return *(static_cast<Concrete_Iterator*>(this));
}

template <class Concrete_Iterator, class Node, class Node_Interface>
bool Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator==(const Concrete_Iterator& other) const
{
  return (m_node == other.m_node);
}

template <class Concrete_Iterator, class Node, class Node_Interface>
bool Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator!=(const Concrete_Iterator& other) const
{
  return (m_node != other.m_node);
}

template <class Concrete_Iterator, class Node, class Node_Interface>
Concrete_Iterator& Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator++()
{
  stepForward();
  return (*(static_cast<Concrete_Iterator*>(this)));
}

template <class Concrete_Iterator, class Node, class Node_Interface>
Concrete_Iterator& Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator--()
{
  stepBack();
  return *(static_cast<Concrete_Iterator*>(this));
}

template <class Concrete_Iterator, class Node, class Node_Interface>
Concrete_Iterator Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator++(int)
{
  Concrete_Iterator cpy(*(static_cast<Concrete_Iterator*>(this)));
  operator++();
  return cpy;
}

template <class Concrete_Iterator, class Node, class Node_Interface>
Concrete_Iterator
Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator--(int)
{
  Concrete_Iterator cpy(*(static_cast<Concrete_Iterator*>(this)));
  operator--();
  return cpy;
}

template <class Concrete_Iterator, class Node, class Node_Interface>
Concrete_Iterator& Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator+=(size_t steps)
{
  for (std::size_t i = 0u; i < steps; ++i)
  {
    this->operator++();
  }

  return (*(static_cast<Concrete_Iterator*>(this)));
}

template <class Concrete_Iterator, class Node, class Node_Interface>
Concrete_Iterator Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator+(size_t steps) const
{
  Concrete_Iterator cpy(m_node);
  cpy += steps;
  return cpy;
}

template <class Concrete_Iterator, class Node, class Node_Interface>
Concrete_Iterator& Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator-=(size_t steps)
{
  for (std::size_t i = 0u; i < steps; ++i)
  {
    this->operator--();
  }

  return (*(static_cast<Concrete_Iterator*>(this)));
}

template <class Concrete_Iterator, class Node, class Node_Interface>
Concrete_Iterator Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator-(size_t steps) const
{
  Concrete_Iterator cpy(m_node);
  cpy -= steps;
  return cpy;
}

template<class Concrete_Iterator, class Node, class Node_Interface>
Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::Const_Reference Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator*() const
{
  return *(m_node);
}

template<class Concrete_Iterator, class Node, class Node_Interface>
Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::Const_Pointer Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator->() const
{
  return m_node;
}

} // namespace crude