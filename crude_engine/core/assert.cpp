module crude.core.assert;

#include <stdexcept>

import crude.core.debug;

namespace crude::core
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