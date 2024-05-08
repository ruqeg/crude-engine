#include <stdexcept>

module crude_engine.core.assert;

namespace crude_engine
{

template<class T>
void assert(T&& assertion) noexcept requires(gDebug)
{
  if (!static_cast<bool>(assertion))
  {
    staticAssert(false);
    return;
  }

  if (!assertion) 
  {
    throw std::exception();
  }
}

template<class T>
void staticAssert(T&& assertion)
{
  static_assert(assertion);
}

}