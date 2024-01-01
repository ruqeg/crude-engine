#include <core/data_structures/array_dynamic.hpp>

namespace crude_engine
{

template<class T>
Array_Dynamic<T>::Array_Dynamic()
  :
  m_data(nullptr),
  m_size(0u)
{}

template<class T>
Array_Dynamic<T>::Array_Dynamic(Size_Type size)
  :
  m_size(size)
{
  m_data = Memory_System::mnewFRBTA<T>(size);
}

template<class T>
Array_Dynamic<T>::Array_Dynamic(Size_Type size, Const_Reference value)
  :
  m_size(size)
{
  m_data = Memory_System::mnewFRBTA<T>(m_size);

  Algorithms::fill(begin(), end(), value);
}

template<class T>
Array_Dynamic<T>::~Array_Dynamic()
{
  if (m_data)
  {
    Memory_System::mdeleteFRBTA<T>(m_data);
  }
}

template<class T>
Array_Dynamic<T>::Array_Dynamic(std::initializer_list<Value_Type>& initList)
  :
  m_size(size)
{
  m_data = Memory_System::mnewFRBTA<T>(initList.size());

  Algorithms::copy(initList.begin(), initList.end(), begin());
}

template<class T>
Array_Dynamic<T>::Array_Dynamic(const Array_Dynamic& other)
  :
  m_size(size)
{
  m_data = Memory_System::mnewFRBTA<T>(m_size);

  Algorithms::copy(other.begin(), other.end(), begin());
}

template<class T>
Array_Dynamic<T>::Array_Dynamic(Array_Dynamic&& other)
{
  m_data = other.m_data;
  m_size = other.m_size;

  other.m_data = nullptr;
}


template<class T>
Array_Dynamic<T>& Array_Dynamic<T>::operator=(const Array_Dynamic<T>& other)
{
  m_size = other.size;
  m_data = Memory_System::mnewFRBTA<T>(m_size);

  Algorithms::copy(other.begin(), other.end(), begin());
}

template<class T>
Array_Dynamic<T>& Array_Dynamic<T>::operator=(Array_Dynamic<T>&& other)
{
  m_data = other.m_data;
  m_size = other.m_size;

  other.m_data = nullptr;
}

template<class T>
void Array_Dynamic<T>::resize(Size_Type newSize) noexcept
{
  Pointer newData = Memory_System::mnewFRBTA<T>(newSize);

  if (m_data)
  {
    Algorithms::copy(begin(), end(), Iterator(newData));
    Memory_System::mdeleteFRBTA<T>(m_data);
  }

  m_data = newData;
  m_size = newSize;
}

template<class T>
Array_Dynamic<T>::Pointer Array_Dynamic<T>::data() noexcept
{
  return m_data;
}

template<class T>
Array_Dynamic<T>::Const_Pointer Array_Dynamic<T>::data() const noexcept
{
  return m_data;
}

template<class T>
Array_Dynamic<T>::Reference Array_Dynamic<T>::get(std::size_t index) noexcept
{
  CRUDE_ASSERT("index out of range" && index < m_size);
  return m_data[index];
}

template<class T>
Array_Dynamic<T>::Const_Reference Array_Dynamic<T>::get(std::size_t index) const noexcept
{
  CRUDE_ASSERT("Array_Stack::get index out of range" && index < m_size);
  return m_data[index];
}

template<class T>
void Array_Dynamic<T>::set(std::size_t index, Const_Reference value) noexcept
{
  CRUDE_ASSERT("Array_Stack::set index out of range" && index < m_size);
  m_data[index] = value;
}

template<class T>
Array_Dynamic<T>::Reference Array_Dynamic<T>::operator[](std::size_t index) noexcept
{
  CRUDE_ASSERT("Array_Stack::operator[] index out of range" && index < m_size);
  return m_data[index];
}

template<class T>
Array_Dynamic<T>::Const_Reference Array_Dynamic<T>::operator[](std::size_t index) const noexcept
{
  CRUDE_ASSERT("Array_Stack::operator[] index out of range" && index < m_size);
  return m_data[index];
}

template<class T>
Array_Dynamic<T>::Size_Type Array_Dynamic<T>::size() const noexcept
{
  return m_size;
}

template<class T>
void Array_Dynamic<T>::fill(Const_Reference value) noexcept
{
  Algorithms::fill(begin(), end(), value);
}

template<class T>
Array_Dynamic<T>::Reference Array_Dynamic<T>::front() noexcept
{
  return m_data[0];
}

template<class T>
Array_Dynamic<T>::Const_Reference Array_Dynamic<T>::front() const noexcept
{
  return m_data[0];
}

template<class T>
Array_Dynamic<T>::Reference Array_Dynamic<T>::back() noexcept
{
  return m_data[m_size - 1];
}

template<class T>
Array_Dynamic<T>::Const_Reference Array_Dynamic<T>::back() const noexcept
{
  return m_data[m_size - 1];
}

template<class T>
Array_Dynamic<T>::Const_Iterator Array_Dynamic<T>::cbegin() const noexcept
{
  return Const_Iterator(m_data[0]);
}

template<class T>
Array_Dynamic<T>::Const_Iterator Array_Dynamic<T>::cend() const noexcept
{
  return Const_Iterator(m_data[m_size]);
}

template<class T>
Array_Dynamic<T>::Const_Iterator Array_Dynamic<T>::begin() const noexcept
{
  return cbegin();
}

template<class T>
Array_Dynamic<T>::Iterator Array_Dynamic<T>::begin() noexcept
{
  return Iterator(m_data);
}

template<class T>
Array_Dynamic<T>::Const_Iterator Array_Dynamic<T>::end() const noexcept
{
  return cend();
}

template<class T>
Array_Dynamic<T>::Iterator Array_Dynamic<T>::end() noexcept
{
  return Iterator(m_data + m_size);
}

template<class T>
Array_Dynamic<T>::Const_Iterator Array_Dynamic<T>::crbegin() const noexcept
{
  return Const_Iterator(m_data);
}

template<class T>
Array_Dynamic<T>::Const_Iterator Array_Dynamic<T>::crend() const noexcept
{
  return Const_Iterator(m_data + (m_size - 1));
}

template<class T>
Array_Dynamic<T>::Const_Iterator Array_Dynamic<T>::rbegin() const noexcept
{
  return crbegin();
}

template<class T>
Array_Dynamic<T>::Iterator Array_Dynamic<T>::rbegin() noexcept
{
  return Iterator(m_data);
}

template<class T>
Array_Dynamic<T>::Const_Iterator Array_Dynamic<T>::rend() const noexcept
{
  return crend();
}

template<class T>
Array_Dynamic<T>::Iterator Array_Dynamic<T>::rend() noexcept
{
  return Iterator(m_data + (m_size - 1));
}

} // namespace crude_engine