module;

export module crude_engine.algorithms;

import crude_engine.alias;

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

}