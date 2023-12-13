#pragma once

#include <core/alias.hpp>

namespace crude_engine
{

template<bool enable>
class Size_Holder {};

template<>
class Size_Holder<true> {
public:
  Size_Holder() : m_size(0u) {};
  
  void add(std::size_t i)
  {
    m_size += i;
  }

  void reduce(std::size_t i)
  {
    m_size -= i;
  }

  std::size_t get() const
  {
    return m_size;
  }

  void set(std::size_t i)
  {
    m_size = i;
  }

private:
  std::size_t m_size;
};

template<>
class Size_Holder<false>
{
public:
  void add(std::size_t i) {}
  void reduce(std::size_t i) {}
  void set(std::size_t i) {}
};

} // namespace crude_engine
