#include <core/data_structures/iterator_base.hpp>

namespace crude_engine
{

template<class T>
void crude_engine::Iterator_Interface_Base<T>::stepForward(T*& ptr) noexcept
{
  ++ptr;
}

template<class T>
void crude_engine::Iterator_Interface_Base<T>::stepBack(T*& ptr) noexcept
{
  --ptr;
}

template<class T>
void Iterator_Interface_Base<T>::jumpForward(T*& ptr, std::size_t steps) noexcept
{
  ptr += steps;
}

template<class T>
void Iterator_Interface_Base<T>::jumpBack(T*& ptr, std::size_t steps) noexcept
{
  ptr -= steps;
}

template<class Concrete_Iterator, class Type, class Interface>
Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::Contiguous_Iterator_Base() noexcept
  :
  m_ptr(nullptr)
{}

template<class Concrete_Iterator, class Type, class Interface>
Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::Contiguous_Iterator_Base(Pointer ptr) noexcept
  :
  m_ptr(ptr)
{}

template<class Concrete_Iterator, class Type, class Interface>
Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::Contiguous_Iterator_Base(const Concrete_Iterator& other) noexcept
  :
  m_ptr(other.m_ptr)
{}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator& Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator=(const Concrete_Iterator& other) noexcept
{
  this->m_ptr = other.m_ptr;
  return *(static_cast<Concrete_Iterator*>(this));
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator& Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator=(Concrete_Iterator&& other) noexcept
{
  this->m_ptr = other.m_ptr;
  return *(static_cast<Concrete_Iterator*>(this));
}

template<class Concrete_Iterator, class Type, class Interface>
bool Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator==(const Concrete_Iterator& other) const noexcept
{
  return (this->m_ptr == other.m_ptr);
}

template<class Concrete_Iterator, class Type, class Interface>
bool Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator!=(const Concrete_Iterator& other) const noexcept
{
  return (this->m_ptr != other.m_ptr);
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator& Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator++() noexcept
{
  Interface::stepForward(m_ptr);
  return (*(static_cast<Concrete_Iterator*>(this)));
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator++(int) noexcept
{
  Concrete_Iterator cpy(*(static_cast<Concrete_Iterator*>(this)));
  operator++();
  return cpy;
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator& Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator+=(std::size_t steps) noexcept
{
  Interface::jumpForward(m_ptr, steps);
  return (*(static_cast<Concrete_Iterator*>(this)));
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator+(std::size_t steps) const noexcept
{
  Concrete_Iterator cpy(m_ptr);
  cpy += steps;
  return cpy;
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator& Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator--() noexcept
{
  Interface::stepBack(m_ptr);
  return *(static_cast<Concrete_Iterator*>(this));
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator--(int) noexcept
{
  Concrete_Iterator cpy(*(static_cast<Concrete_Iterator*>(this)));
  operator--();
  return cpy;
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator& Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator-=(std::size_t steps) noexcept
{
  Interface::jumpBack(m_ptr, steps);
  return (*(static_cast<Concrete_Iterator*>(this)));
}

template<class Concrete_Iterator, class Type, class Interface>
Concrete_Iterator Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator-(std::size_t steps) const noexcept
{
  Concrete_Iterator cpy(m_ptr);
  cpy -= steps;
  return cpy;
}

template<class Concrete_Iterator, class Type, class Interface>
Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::Reference Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator*() noexcept
{
  return *(m_ptr);
}

template<class Concrete_Iterator, class Type, class Interface>
Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::Pointer Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator->() noexcept
{
  return m_ptr;
}

template<class Concrete_Iterator, class Type, class Interface>
Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::Const_Reference Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator*() const noexcept
{
  return *(m_ptr);
}

template<class Concrete_Iterator, class Type, class Interface>
Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::Const_Pointer Contiguous_Iterator_Base<Concrete_Iterator, Type, Interface>::operator->() const noexcept
{
  return *m_ptr;
}

} // namespace crude_engine