#include <core/data_structures/list.hpp>

namespace crude_engine
{

template<class Node>
List_Node_Base<Node>::List_Node_Base() noexcept
  :
  m_next(nullptr),
  m_prev(nullptr)
{}

template<class Node>
void List_Node_Base<Node>::setNext(Node* newNext) noexcept
{
  m_next = newNext;
}

template<class Node>
void List_Node_Base<Node>::setPrev(Node* newPrev) noexcept
{
  m_prev = newPrev;
}

template<class Node>
Node*& List_Node_Base<Node>::getNext() const noexcept
{
  return m_next;
}

template<class Node>
Node*& List_Node_Base<Node>::getPrev() const noexcept
{
  return m_prev;
}

template<class Node, class Compare>
List<Node, Compare>::List() noexcept
  :
  m_first(nullptr),
  m_last(nullptr)
{}

template<class Node, class Compare>
List<Node, Compare>::List(List&& other) noexcept
  :
  m_first(other.m_first),
  m_last(other.m_last),
  m_size(other.m_size)
{}

template<class Node, class Compare>
List<Node, Compare>& List<Node, Compare>::operator=(List&& other) noexcept
{
  m_first = other.m_first;
  m_last = other.m_last;
  m_size = other.m_size;
}

} // namespace crude_engine