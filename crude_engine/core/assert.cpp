module crude.core.assert;

#include <stdexcept>
#include <assert.h>

import crude.core.debug;

namespace crude::core
{

void (assert)(bool assertion) noexcept
{
  assert(assertion);
}

}