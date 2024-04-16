#include <core/string.hpp>

namespace crude_engine
{

template<typename ...Args>
char* String::snprintf(char* const buffer, const std::size_t bufferCount, const char* const format, Args && ...args) noexcept
{
  ::snprintf(buffer, bufferCount, format, Utility::forward<Args>(args)...);
  return buffer;
}

}