#include <core/data_structures/iterator_base.hpp>

namespace crude_engine
{

template<class T>
void crude_engine::Iterator_Interface_Base<T>::stepForward(T*& ptr)
{
  ++ptr;
}

template<class T>
void crude_engine::Iterator_Interface_Base<T>::stepBack(T*& ptr)
{
  --ptr;
}

template<class Concrete_Iterator, class Type, class Interface>
Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::Contiguous_Iterator_Base()
  :
  m_ptr(nullptr)
{}

template<class Concrete_Iterator, class Type, class Interface>
Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::Contiguous_Iterator_Base(Pointer ptr)
  :
  m_ptr(ptr)
{}

template<class Concrete_Iterator, class Type, class Interface>
Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::Contiguous_Iterator_Base(const Concrete_Iterator& other)
  :
  m_ptr(other.m_ptr)
{}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator& Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator=(const Concrete_Iterator& other)
{
  this->m_ptr = other.m_ptr;
  return *(static_cast<Concrete_Iterator*>(this));
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator& Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator=(Concrete_Iterator&& other)
{
  this->m_ptr = other.m_ptr;
  return *(static_cast<Concrete_Iterator*>(this));
}

template<class Concrete_Iterator, class Type, class Interface>
bool Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator==(const Concrete_Iterator& other) const
{
  return (this->m_ptr == other.m_ptr);
}

template<class Concrete_Iterator, class Type, class Interface>
bool Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator!=(const Concrete_Iterator& other) const
{
  return (this->m_ptr != other.m_ptr);
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator& Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator++()
{
  Interface::stepForward(m_ptr);
  return (*(static_cast<Concrete_Iterator*>(this)));
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator++(int)
{
  Concrete_Iterator cpy(*(static_cast<Concrete_Iterator*>(this)));
  operator++();
  return cpy;
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator& Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator+=(std::size_t steps)
{
  for (std::size_t i = 0u; i < steps; ++i)
  {
    this->operator++();
  }

  return (*(static_cast<Concrete_Iterator*>(this)));
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator+(std::size_t steps) const
{
  Concrete_Iterator cpy(m_ptr);
  cpy += steps;
  return cpy;
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator& Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator--()
{
  Interface::stepBack(m_ptr);
  return *(static_cast<Concrete_Iterator*>(this));
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator--(int)
{
  Concrete_Iterator cpy(*(static_cast<Concrete_Iterator*>(this)));
  operator--();
  return cpy;
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator& Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator-=(std::size_t steps)
{
  for (std::size_t i = 0u; i < steps; ++i)
  {
    this->operator--();
  }

  return (*(static_cast<Concrete_Iterator*>(this)));
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator-(std::size_t steps) const
{
  Concrete_Iterator cpy(m_ptr);
  cpy -= steps;
  return cpy;
}

template<class Concrete_Iterator, class Type, class Interface>
Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::Reference Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator*()
{
  return *(m_ptr);
}

template<class Concrete_Iterator, class Type, class Interface>
Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::Pointer Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator->()
{
  return m_ptr;
}

template<class Concrete_Iterator, class Type, class Interface>
Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::Const_Reference Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator*() const
{
  return *(m_ptr);
}

template<class Concrete_Iterator, class Type, class Interface>
Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::Const_Pointer Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator->() const
{
  return *m_ptr;
}

} // namespace crude_engine