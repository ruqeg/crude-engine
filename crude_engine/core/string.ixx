module;

export module crude_engine.string;

import crude_engine.alias;
import crude_engine.utility;

export namespace crude_engine
{

size_t strlen(char* const buffer) noexcept;
char* strncat(char* const buffer, const size_t bufferCount, const char* const source) noexcept;
template<typename... Args>
char* snprintf(char* const buffer, const size_t bufferCount, const char* const format, Args&&... args) noexcept;

}