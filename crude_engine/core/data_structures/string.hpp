#pragma once

#include <core/utility.hpp>
#include <stdio.h>

namespace crude_engine
{

class String
{
public:
  static std::size_t strlen(char* const buffer) noexcept;
  static char* strncat(char* const buffer, const std::size_t bufferCount, const char* const source) noexcept;
  template<typename... Args>
  static char* snprintf(char* const buffer, const std::size_t bufferCount, const char* const format, Args&&... args) noexcept;
};

}

#include <core/data_structures/string.inl>