#include <core/data_structures/list_iterator_base.hpp>

namespace crude_engine
{

template<class Node>
void List_Iterator_Interface_Base<Node>::stepForward(Node*& ptr) noexcept
{
  *ptr = ptr->getNext();
}

template<class Node>
void List_Iterator_Interface_Base<Node>::stepBack(Node*& ptr) noexcept
{
  *ptr = ptr->getPrev();
}

template<class Node>
void List_Iterator_Interface_Base<Node>::jumpForward(Node*& ptr, std::size_t steps) noexcept
{
  Node* nptr = ptr;
  for (std::size_t i = 0u; i < steps; ++i)
  {
    CRUDE_ASSERT(nptr);
    nptr = nptr->getNext();
  }
  *ptr = nptr;
}

template<class Node>
void List_Iterator_Interface_Base<Node>::jumpBack(Node*& ptr, std::size_t steps) noexcept
{
  Node* nptr = ptr;
  for (std::size_t i = 0u; i < steps; ++i)
  {
    CRUDE_ASSERT(nptr);
    nptr = nptr->getPrev();
  }
  *ptr = nptr;
}

template<class Concrete_Iterator, class Node, class Interface>
List_Iterator_Base<Concrete_Iterator, Node, Interface>::List_Iterator_Base() noexcept
  :
  Bidirectional_Iterator_Base()
{}

template<class Concrete_Iterator, class Node, class Interface>
List_Iterator_Base<Concrete_Iterator, Node, Interface>::List_Iterator_Base(Pointer ptr) noexcept
  :
  Bidirectional_Iterator_Base(ptr)
{
}

template<class Concrete_Iterator, class Node, class Interface>
List_Iterator_Base<Concrete_Iterator, Node, Interface>::List_Iterator_Base(const Concrete_Iterator& other) noexcept
  :
  Bidirectional_Iterator_Base(other)
{}

} // namespace crude_engine