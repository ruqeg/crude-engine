module crude_engine.core.assert;

#include <stdexcept>

import crude_engine.core.debug;

namespace crude_engine
{

void assert(bool assertion) noexcept
{
  if constexpr (gDebug)
  {
    if (!assertion)
    {
      throw std::exception();
    }
  }
}

}