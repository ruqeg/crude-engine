module;

export module crude.core.size_holder;

import crude.core.alias;

export namespace crude::core
{

template<bool enable>
class Size_Holder
{};

template<>
class Size_Holder<true>
{
public:
  using Size_Type = size_t;

public:
  Size_Holder() : m_size(0u) {};
  
  void add(Size_Type i)
  {
    m_size += i;
  }

  void reduce(Size_Type i)
  {
    m_size -= i;
  }

  Size_Type get() const
  {
    return m_size;
  }

  void set(Size_Type i)
  {
    m_size = i;
  }

private:
  Size_Type m_size;
};

template<>
class Size_Holder<false>
{
public:
  using Size_Type = size_t;

public:
  void add(Size_Type) {}
  void reduce(Size_Type) {}
  void set(Size_Type) {}
};

}