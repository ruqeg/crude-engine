#include <core/string.hpp>

namespace crude_engine
{

std::size_t String::strlen(char* const buffer) noexcept
{
  std::size_t len = 0u;

  while (buffer[len] != '\0')
  {
    len++;
  }

  return len;
}

char* String::strncat(char* const buffer, const std::size_t bufferCount, const char* const source) noexcept
{
  size_t bufferLen = strlen(buffer);
  size_t i;

  for (i = 0u; (i < bufferCount) && (source[i] != '\0'); i++)
  {
    buffer[bufferLen + i] = source[i];
  }
  buffer[bufferLen + i] = '\0';

  return buffer;
}

}