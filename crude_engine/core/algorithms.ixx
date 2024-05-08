module;

export module crude_engine.core.algorithms;

export namespace crude_engine
{

class Algorithms
{
public:
  template<class Forward_Iterator, class T>
  static void fill(Forward_Iterator first, Forward_Iterator last, const T& value);

  template<class Input_Iterator, class Output_Iterator>
  static Output_Iterator copy(Input_Iterator first, Input_Iterator last, Output_Iterator dFirst);

  template<class Input_Iterator, class Output_Iterator, class Function>
  static Output_Iterator copyc(Input_Iterator first, Input_Iterator last, Output_Iterator dFirst, const Function& func);
};

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