#include <Windows.h>

module crude_engine.core.logger;

import crude_engine.core.string;
import crude_engine.core.utility;

namespace crude_engine
{

template<typename... Args>
inline void log(const Debug::Channel channel, const char* format, Args&&... args)
{
  Log_Object(__FILE__, __LINE__)(channel, format, forward<Args>(args)...);
}

template<typename... Args>
inline void logInfo(const Debug::Channel channel, const char* format, Args&&... args)
{
  Log_Object(__FILE__, __LINE__, Debug::Verbosity::Info)(channel, format, forward<Args>(args)...);
}

template<typename... Args>
inline void logWarning(const Debug::Channel channel, const char* format, Args&&... args)
{
  Log_Object(__FILE__, __LINE__, Debug::Verbosity::Warning)(channel, format, forward<Args>(args)...);
}

template<typename... Args>
inline void logError(const Debug::Channel channel, const char* format, Args&&... args)
{
  Log_Object(__FILE__, __LINE__, Debug::Verbosity::Error)(channel, format, forward<Args>(args)...);
}

Log_Object::Log_Object(const char* filename, const int32 line)
  :
  m_filename(filename),
  m_line(line),
  m_verbosity(Debug::Verbosity::Info)
{}

Log_Object::Log_Object(const char* filename, const int32 line, Debug::Verbosity verbosity)
  :
  m_filename(filename),
  m_line(line),
  m_verbosity(verbosity)
{}

void Logger::outputStr(const char* str)
{
  OutputDebugStringA((LPCSTR)str);
}

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

  strncat(formatBuffer, sizeof(formatBuffer), "[channel: %s][verbosity: %s][filename: %s][line: %i] =>\n\t");
  strncat(formatBuffer, sizeof(formatBuffer), format);
  strncat(formatBuffer, sizeof(formatBuffer), "\n");
  snprintf(messageBuffer, sizeof(messageBuffer), formatBuffer, Debug::toString(channel), Debug::toString(verbosity), filename, line, forward<Args>(args)...);
  outputStr(messageBuffer);
}

template<typename ...Args>
void Log_Object::operator()(const Debug::Channel  channel,
                            const char*           format,
                            Args&&...             args) const noexcept
{
  Logger::debugPrintF(m_filename, m_line, channel, m_verbosity, format, forward<Args>(args)...);
}

}