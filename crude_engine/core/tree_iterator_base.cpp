#include <iterator>

module crude_engine.tree_iterator_base;

namespace crude_engine
{

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
Concrete_Iterator& Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator+=(std::size_t steps)
{
  for (std::size_t i = 0u; i < steps; ++i)
  {
    this->operator++();
  }

  return (*(static_cast<Concrete_Iterator*>(this)));
}

template <class Concrete_Iterator, class Node, class Node_Interface>
Concrete_Iterator Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator+(std::size_t steps) const
{
  Concrete_Iterator cpy(m_node);
  cpy += steps;
  return cpy;
}

template <class Concrete_Iterator, class Node, class Node_Interface>
Concrete_Iterator& Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator-=(std::size_t steps)
{
  for (std::size_t i = 0u; i < steps; ++i)
  {
    this->operator--();
  }

  return (*(static_cast<Concrete_Iterator*>(this)));
}

template <class Concrete_Iterator, class Node, class Node_Interface>
Concrete_Iterator Tree_Iterator_Base<Concrete_Iterator, Node, Node_Interface>::operator-(std::size_t steps) const
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

} // namespace crude_engine