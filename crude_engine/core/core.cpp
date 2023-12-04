#include "core.hpp"
#include <cstring>

namespace crude_engine
{

void Core::copyArray(int8* dst, const int8* src, uint32 n)
{
  for (uint64 i = 0u; i < n; ++i)
  {
    dst[i] = src[i];
  }
}

int8* Core::initCopyArray(const int8* src, uint32 n)
{
  if (src == nullptr)
  {
    return nullptr;
  }

  int8* dst = CRUDE_VULKAN_01_NEW int8[n];
  copyArray(dst, src, n);
  return dst;
}

char8* Core::initCopyString(const char8* src)
{
  if (src == nullptr)
  {
    return nullptr;
  }

  size_t count = strlen(src) + 1;
  char8* newstr = CRUDE_VULKAN_01_NEW char8[count];
  const errno_t err = strcpy_s(newstr, count, src);
  CRUDE_VULKAN_01_ASSERT(err == 0);
  return newstr;
}

}
