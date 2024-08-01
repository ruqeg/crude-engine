module;

#include <cstring>

export module crude.core.algorithms;

export import crude.core.memory;
export import crude.core.alias;

export namespace crude::core
{

template<class Forward_Iterator, class T>
void fill(Forward_Iterator first, Forward_Iterator last, const T& value) noexcept
{
  while (first != last)
  {
    *first = value;
    ++first;
  }
}

template<class Input_Iterator, class Output_Iterator>
Output_Iterator copy(Input_Iterator first, Input_Iterator last, Output_Iterator dFirst) noexcept
{
  while (first != last)
  {
    *dFirst = *first;
    ++dFirst; ++first;
  }
  return dFirst;
}

template<class Input_Iterator, class Output_Iterator, class Function>
Output_Iterator copyc(Input_Iterator first, Input_Iterator last, Output_Iterator dFirst, const Function& func) noexcept
{
  while (first != last)
  {
    func(first, dFirst);
    ++dFirst; ++first;
  }
  return dFirst;
}

template<class T>
T* cxxDefaultAllocateCopy(const T* src, core::size_t size) noexcept
{
  T* dst = nullptr;
  if (src && size)
  {
    dst = defaultCxxAllocate<T>(size);
    if (dst)
    {
      std::memcpy(dst, src, sizeof(T) * size);
    }
  }
  return dst;
}

}