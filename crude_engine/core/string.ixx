module;

#include <utility>
#include <stdio.h>

export module crude_engine.core.string;

export import crude_engine.core.alias;

import crude_engine.core.utility;

export namespace crude_engine
{

size_t strlen(char* const buffer) noexcept;
char* strncat(char* const buffer, const size_t bufferCount, const char* const source) noexcept;
template<typename... Args>
char* snprintf(char* const buffer, const size_t bufferCount, const char* const format, Args&&... args) noexcept
{
  ::snprintf(buffer, bufferCount, format, std::forward<Args>(args)...);
  return buffer;
}

}