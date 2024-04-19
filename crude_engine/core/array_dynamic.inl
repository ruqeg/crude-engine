#include <core/array_dynamic.hpp>

namespace crude_engine
{

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Array_Dynamic()
  :
  m_data(nullptr),
  m_size(0u)
{}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Array_Dynamic(Size_Type size)
  :
  m_size(size)
{
  m_data = Allocator::mnewArray<T>(m_size);
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Array_Dynamic(Size_Type size, Const_Reference value)
  :
  m_size(size)
{
  m_data = Allocator::mnewArray<T>(m_size, value);
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::~Array_Dynamic()
{
  if (m_data)
  {
    Allocator::mdeleteArray<T>(m_size, m_data);
  }
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Array_Dynamic(std::initializer_list<Value_Type> initList)
  :
  m_size(initList.size())
{
  m_data = reinterpret_cast<Pointer>(Allocator::allocate(sizeof(T) * m_size));

  Algorithms::copyc(initList.begin(), initList.end(), begin(), [](auto src, auto dst) -> void {
    Utility::constructAt(&(*dst), *src);
  });
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Array_Dynamic(const Array_Dynamic& other)
  :
  m_size(other.m_size)
{
  m_data = reinterpret_cast<Pointer>(Allocator::allocate(sizeof(T) * m_size));

  Algorithms::copyc(other.begin(), other.end(), begin(), [](auto src, auto dst) -> void {
    Utility::constructAt(&(*dst), *src);
  });
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Array_Dynamic(Array_Dynamic&& other)
  :
  m_data(other.m_data),
  m_size(other.m_size)
{
  other.m_data = nullptr;
  other.m_size = 0u;
}


template<class T, class Allocator>
Array_Dynamic<T, Allocator>& Array_Dynamic<T, Allocator>::operator=(const Array_Dynamic<T, Allocator>& other)
{
  m_size = other.m_size;

  m_data = reinterpret_cast<Pointer>(Allocator::allocate(sizeof(T) * m_size));

  Algorithms::copyc(other.begin(), other.end(), begin(), [](auto src, auto dst) -> void {
    Utility::constructAt(&(*dst), *src);
  });

  return *this;
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>& Array_Dynamic<T, Allocator>::operator=(Array_Dynamic<T, Allocator>&& other)
{
  m_data = other.m_data;
  m_size = other.m_size;

  other.m_data = nullptr;
  other.m_size = 0u;

  return *this;
}

template<class T, class Allocator>
void Array_Dynamic<T, Allocator>::resize(Size_Type newSize) noexcept
{
  Pointer newData;
  if (newSize == 0u)
  {
    newData = nullptr;
  }
  else
  {
    newData = Allocator::mnewArray<T>(newSize);
  }

  if (m_data)
  {
    if (newData)
      Algorithms::copy(begin(), end(), Iterator(newData));

    Allocator::mdeleteArray<T>(m_size, m_data);
  }

  m_data = newData;
  m_size = newSize;
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Pointer Array_Dynamic<T, Allocator>::data() noexcept
{
  return m_data;
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Const_Pointer Array_Dynamic<T, Allocator>::data() const noexcept
{
  return m_data;
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Reference Array_Dynamic<T, Allocator>::get(std::size_t index) noexcept
{
  CRUDE_ASSERT("index out of range" && index < m_size);
  return m_data[index];
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Const_Reference Array_Dynamic<T, Allocator>::get(std::size_t index) const noexcept
{
  CRUDE_ASSERT("Array_Stack::get index out of range" && index < m_size);
  return m_data[index];
}

template<class T, class Allocator>
void Array_Dynamic<T, Allocator>::set(std::size_t index, Const_Reference value) noexcept
{
  CRUDE_ASSERT("Array_Stack::set index out of range" && index < m_size);
  m_data[index] = value;
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Reference Array_Dynamic<T, Allocator>::operator[](std::size_t index) noexcept
{
  CRUDE_ASSERT("Array_Stack::operator[] index out of range" && index < m_size);
  return m_data[index];
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Const_Reference Array_Dynamic<T, Allocator>::operator[](std::size_t index) const noexcept
{
  CRUDE_ASSERT("Array_Stack::operator[] index out of range" && index < m_size);
  return m_data[index];
}

template<class T, class Allocator>
void Array_Dynamic<T, Allocator>::clear()
{
  resize(0u);
}

template<class T, class Allocator>
bool Array_Dynamic<T, Allocator>::empty() const noexcept
{
  return (m_size == 0u);
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Size_Type Array_Dynamic<T, Allocator>::size() const noexcept
{
  return m_size;
}

template<class T, class Allocator>
void Array_Dynamic<T, Allocator>::fill(Const_Reference value) noexcept
{
  Algorithms::fill(begin(), end(), value);
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Reference Array_Dynamic<T, Allocator>::front() noexcept
{
  return m_data[0];
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Const_Reference Array_Dynamic<T, Allocator>::front() const noexcept
{
  return m_data[0];
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Reference Array_Dynamic<T, Allocator>::back() noexcept
{
  return m_data[m_size - 1];
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Const_Reference Array_Dynamic<T, Allocator>::back() const noexcept
{
  return m_data[m_size - 1];
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Const_Iterator Array_Dynamic<T, Allocator>::cbegin() const noexcept
{
  return Const_Iterator(m_data);
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Const_Iterator Array_Dynamic<T, Allocator>::cend() const noexcept
{
  return Const_Iterator(m_data + m_size);
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Const_Iterator Array_Dynamic<T, Allocator>::begin() const noexcept
{
  return cbegin();
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Iterator Array_Dynamic<T, Allocator>::begin() noexcept
{
  return Iterator(m_data);
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Const_Iterator Array_Dynamic<T, Allocator>::end() const noexcept
{
  return cend();
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Iterator Array_Dynamic<T, Allocator>::end() noexcept
{
  return Iterator(m_data + m_size);
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Const_Iterator Array_Dynamic<T, Allocator>::crbegin() const noexcept
{
  return Const_Iterator(m_data);
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Const_Iterator Array_Dynamic<T, Allocator>::crend() const noexcept
{
  return Const_Iterator(m_data + (m_size - 1));
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Const_Iterator Array_Dynamic<T, Allocator>::rbegin() const noexcept
{
  return crbegin();
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Iterator Array_Dynamic<T, Allocator>::rbegin() noexcept
{
  return Iterator(m_data);
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Const_Iterator Array_Dynamic<T, Allocator>::rend() const noexcept
{
  return crend();
}

template<class T, class Allocator>
Array_Dynamic<T, Allocator>::Iterator Array_Dynamic<T, Allocator>::rend() noexcept
{
  return Iterator(m_data + (m_size - 1));
}

} // namespace crude_engine