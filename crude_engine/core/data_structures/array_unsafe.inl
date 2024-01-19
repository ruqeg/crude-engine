#include <core/data_structures/array_unsafe.hpp>

namespace crude_engine
{

template<class T>
Array_Unsafe<T>::Array_Unsafe()
  :
  m_data(nullptr),
  m_size(0u)
{}

template<class T>
Array_Unsafe<T>::Array_Unsafe(Pointer data, Size_Type size)
  :
  m_data(data),
  m_size(size)
{}

template<class T>
Array_Unsafe<T>::Array_Unsafe(const Array_Unsafe& other)
{
  m_data = other.m_data;
  m_size = other.m_size;
}

template<class T>
Array_Unsafe<T>::Array_Unsafe(Array_Unsafe&& other)
{
  m_data = other.m_data;
  m_size = other.m_size;
}

template<class T>
Array_Unsafe<T>& Array_Unsafe<T>::operator=(const Array_Unsafe<T>& other)
{
  m_data = other.m_data;
  m_size = other.m_size;
  return *this;
}

template<class T>
Array_Unsafe<T>& Array_Unsafe<T>::operator=(Array_Unsafe<T>&& other)
{
  m_data = other.m_data;
  m_size = other.m_size;
  return *this;
}

template<class T>
Array_Unsafe<T>::Pointer Array_Unsafe<T>::data() noexcept
{
  return m_data;
}

template<class T>
Array_Unsafe<T>::Const_Pointer Array_Unsafe<T>::data() const noexcept
{
  return m_data;
}

template<class T>
Array_Unsafe<T>::Reference Array_Unsafe<T>::get(std::size_t index) noexcept
{
  CRUDE_ASSERT(m_data);
  CRUDE_ASSERT("index out of range" && index < m_size);
  return m_data[index];
}

template<class T>
Array_Unsafe<T>::Const_Reference Array_Unsafe<T>::get(std::size_t index) const noexcept
{
  CRUDE_ASSERT(m_data);
  CRUDE_ASSERT("Array_Unsafe::get index out of range" && index < m_size);
  return m_data[index];
}

template<class T>
void Array_Unsafe<T>::set(std::size_t index, Const_Reference value) noexcept
{
  CRUDE_ASSERT(m_data);
  CRUDE_ASSERT("Array_Unsafe::set index out of range" && index < m_size);
  m_data[index] = value;
}

template<class T>
Array_Unsafe<T>::Reference Array_Unsafe<T>::operator[](std::size_t index) noexcept
{
  CRUDE_ASSERT(m_data);
  CRUDE_ASSERT("Array_Unsafe::operator[] index out of range" && index < m_size);
  return m_data[index];
}

template<class T>
Array_Unsafe<T>::Const_Reference Array_Unsafe<T>::operator[](std::size_t index) const noexcept
{
  CRUDE_ASSERT(m_data);
  CRUDE_ASSERT("Array_Unsafe::operator[] index out of range" && index < m_size);
  return m_data[index];
}

template<class T>
Array_Unsafe<T>::Size_Type Array_Unsafe<T>::size() const noexcept
{
  return m_size;
}

template<class T>
void Array_Unsafe<T>::fill(Const_Reference value) noexcept
{
  CRUDE_ASSERT(m_data);
  Algorithms::fill(begin(), end(), value);
}

template<class T>
Array_Unsafe<T>::Reference Array_Unsafe<T>::front() noexcept
{
  return m_data[0];
}

template<class T>
Array_Unsafe<T>::Const_Reference Array_Unsafe<T>::front() const noexcept
{
  return m_data[0];
}

template<class T>
Array_Unsafe<T>::Reference Array_Unsafe<T>::back() noexcept
{
  return m_data[m_size - 1];
}

template<class T>
Array_Unsafe<T>::Const_Reference Array_Unsafe<T>::back() const noexcept
{
  return m_data[m_size - 1];
}

template<class T>
Array_Unsafe<T>::Const_Iterator Array_Unsafe<T>::cbegin() const noexcept
{
  return Const_Iterator(m_data);
}

template<class T>
Array_Unsafe<T>::Const_Iterator Array_Unsafe<T>::cend() const noexcept
{
  return Const_Iterator(m_data + m_size);
}

template<class T>
Array_Unsafe<T>::Const_Iterator Array_Unsafe<T>::begin() const noexcept
{
  return cbegin();
}

template<class T>
Array_Unsafe<T>::Iterator Array_Unsafe<T>::begin() noexcept
{
  return Iterator(m_data);
}

template<class T>
Array_Unsafe<T>::Const_Iterator Array_Unsafe<T>::end() const noexcept
{
  return cend();
}

template<class T>
Array_Unsafe<T>::Iterator Array_Unsafe<T>::end() noexcept
{
  return Iterator(m_data + m_size);
}

template<class T>
Array_Unsafe<T>::Const_Iterator Array_Unsafe<T>::crbegin() const noexcept
{
  return Const_Iterator(m_data);
}

template<class T>
Array_Unsafe<T>::Const_Iterator Array_Unsafe<T>::crend() const noexcept
{
  return Const_Iterator(m_data + (m_size - 1));
}

template<class T>
Array_Unsafe<T>::Const_Iterator Array_Unsafe<T>::rbegin() const noexcept
{
  return crbegin();
}

template<class T>
Array_Unsafe<T>::Iterator Array_Unsafe<T>::rbegin() noexcept
{
  return Iterator(m_data);
}

template<class T>
Array_Unsafe<T>::Const_Iterator Array_Unsafe<T>::rend() const noexcept
{
  return crend();
}

template<class T>
Array_Unsafe<T>::Iterator Array_Unsafe<T>::rend() noexcept
{
  return Iterator(m_data + (m_size - 1));
}

} // namespace crude_engine