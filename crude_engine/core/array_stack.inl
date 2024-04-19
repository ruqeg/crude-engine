#include <core/array_stack.hpp>

namespace crude_engine
{

template<class T, std::size_t N>
template <typename... Args>
Array_Stack<T, N>::Array_Stack(Args&&... args)
  :
  m_data{ Utility::forward<Args>(args)... }
{}

template<class T, std::size_t N>
Array_Stack<T, N>::Array_Stack(const Array_Stack& other)
{
  Algorithms::copy(other.begin(), other.end(), begin());
}

template<class T, std::size_t N>
Array_Stack<T, N>& Array_Stack<T, N>::operator=(const Array_Stack<T, N>& other)
{
  Algorithms::copy(other.begin(), other.end(), begin());
  return *this;
}

template<class T, std::size_t N>
Array_Stack<T, N>::Pointer Array_Stack<T, N>::data() noexcept
{
  return m_data;
}

template<class T, std::size_t N>
Array_Stack<T, N>::Const_Pointer Array_Stack<T, N>::data() const noexcept
{
  return m_data;
}

template<class T, std::size_t N>
Array_Stack<T, N>::Reference Array_Stack<T, N>::get(std::size_t index) noexcept
{
  CRUDE_ASSERT("index out of range" && index < N);
  return m_data[index];
}

template<class T, std::size_t N>
Array_Stack<T, N>::Const_Reference Array_Stack<T, N>::get(std::size_t index) const noexcept
{
  CRUDE_ASSERT("Array_Stack::get index out of range" && index < N);
  return m_data[index];
}

template<class T, std::size_t N>
void Array_Stack<T, N>::set(std::size_t index, Const_Reference value) noexcept
{
  CRUDE_ASSERT("Array_Stack::set index out of range" && index < N);
  m_data[index] = value;
}

template<class T, std::size_t N>
Array_Stack<T, N>::Reference Array_Stack<T, N>::operator[](std::size_t index) noexcept
{
  CRUDE_ASSERT("Array_Stack::operator[] index out of range" && index < N);
  return m_data[index];
}

template<class T, std::size_t N>
Array_Stack<T, N>::Const_Reference Array_Stack<T, N>::operator[](std::size_t index) const noexcept
{
  CRUDE_ASSERT("Array_Stack::operator[] index out of range" && index < N);
  return m_data[index];
}

template<class T, std::size_t N>
Array_Stack<T, N>::Size_Type Array_Stack<T, N>::size() const noexcept
{
  return static_cast<Size_Type>(N);
}

template<class T, std::size_t N>
void Array_Stack<T, N>::fill(Const_Reference value) noexcept
{
  Algorithms::fill(begin(), end(), value);
}

template<class T, std::size_t N>
Array_Stack<T, N>::Reference Array_Stack<T, N>::front() noexcept
{
  return m_data[0];
}

template<class T, std::size_t N>
Array_Stack<T, N>::Const_Reference Array_Stack<T, N>::front() const noexcept
{
  return m_data[0];
}

template<class T, std::size_t N>
Array_Stack<T, N>::Reference Array_Stack<T, N>::back() noexcept
{
  return m_data[N - 1];
}

template<class T, std::size_t N>
Array_Stack<T, N>::Const_Reference Array_Stack<T, N>::back() const noexcept
{
  return m_data[N - 1];
}

template<class T, std::size_t N>
Array_Stack<T, N>::Const_Iterator Array_Stack<T, N>::cbegin() const noexcept
{
  return Const_Iterator(m_data);
}

template<class T, std::size_t N>
Array_Stack<T, N>::Const_Iterator Array_Stack<T, N>::cend() const noexcept
{
  return Const_Iterator(m_data + N);
}

template<class T, std::size_t N>
Array_Stack<T, N>::Const_Iterator Array_Stack<T, N>::begin() const noexcept
{
  return cbegin();
}

template<class T, std::size_t N>
Array_Stack<T, N>::Iterator Array_Stack<T, N>::begin() noexcept
{
  return Iterator(m_data);
}

template<class T, std::size_t N>
Array_Stack<T, N>::Const_Iterator Array_Stack<T, N>::end() const noexcept
{
  return cend();
}

template<class T, std::size_t N>
Array_Stack<T, N>::Iterator Array_Stack<T, N>::end() noexcept
{
  return Iterator(m_data + N);
}

template<class T, std::size_t N>
Array_Stack<T, N>::Const_Iterator Array_Stack<T, N>::crbegin() const noexcept
{
  return Const_Iterator(m_data);
}

template<class T, std::size_t N>
Array_Stack<T, N>::Const_Iterator Array_Stack<T, N>::crend() const noexcept
{
  return Const_Iterator(m_data + (N - 1));
}

template<class T, std::size_t N>
Array_Stack<T, N>::Const_Iterator Array_Stack<T, N>::rbegin() const noexcept
{
  return crbegin();
}

template<class T, std::size_t N>
Array_Stack<T, N>::Iterator Array_Stack<T, N>::rbegin() noexcept
{
  return Iterator(m_data);
}

template<class T, std::size_t N>
Array_Stack<T, N>::Const_Iterator Array_Stack<T, N>::rend() const noexcept
{
  return crend();
}

template<class T, std::size_t N>
Array_Stack<T, N>::Iterator Array_Stack<T, N>::rend() noexcept
{
  return Iterator(m_data + (N - 1));
}

} // namespace crude_engine