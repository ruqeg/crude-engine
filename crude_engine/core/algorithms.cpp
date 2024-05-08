module crude_engine.algorithms;

namespace crude_engine
{

template<class Forward_Iterator, class T>
void Algorithms::fill(Forward_Iterator first, Forward_Iterator last, const T& value)
{
  while (first != last)
  {
    *first = value;
    ++first;
  }
}

template<class Input_Iterator, class Output_Iterator>
Output_Iterator Algorithms::copy(Input_Iterator first, Input_Iterator last, Output_Iterator dFirst)
{
  while (first != last)
  {
    *dFirst = *first;
    ++dFirst; ++first;
  }
  return dFirst;
}

template<class Input_Iterator, class Output_Iterator, class Function>
Output_Iterator Algorithms::copyc(Input_Iterator first, Input_Iterator last, Output_Iterator dFirst, const Function& func)
{
  while (first != last)
  {
    func(first, dFirst);
    ++dFirst; ++first;
  }
  return dFirst;
}

}