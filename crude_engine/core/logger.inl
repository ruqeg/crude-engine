#include <core/logger.hpp>

namespace crude_engine
{

template<typename... Args>
void Logger::debugPrintF(const char*             filename,
                         const int32             line,
                         const Debug::Channel    channel,
                         const Debug::Verbosity  verbosity,
                         const char*             format,
                         Args&&...               args) noexcept
{
  char messageBuffer[1024] = {};
  char formatBuffer[1024] = {};

  String::strncat(formatBuffer, sizeof(formatBuffer), "[channel: %s][verbosity: %s][filename: %s][line: %i] =>\n\t");
  String::strncat(formatBuffer, sizeof(formatBuffer), format);
  String::strncat(formatBuffer, sizeof(formatBuffer), "\n");
  String::snprintf(messageBuffer, sizeof(messageBuffer), formatBuffer, Debug::toString(channel), Debug::toString(verbosity), filename, line, Utility::forward<Args>(args)...);
  OutputDebugStringA((LPCSTR)messageBuffer);
}

template<typename ...Args>
void Log_Object::operator()(const Debug::Channel    channel,
                            const char*             format,
                            Args&&...               args) const noexcept
{
  Logger::debugPrintF(m_filename, m_line, channel, m_verbosity, format, Utility::forward<Args>(args)...);
}

}