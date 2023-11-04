#include "core.hpp"
#include <cstring>

namespace crude_vulkan_01
{

void Core::copyArray(int8* dst, const int8* src, uint32 n)
{
  for (uint64 i = 0u; i < n; ++i)
  {
    dst[i] = src[i];
  }
}

}
