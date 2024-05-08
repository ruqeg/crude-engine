#include <Windows.h>

module crude_engine.logger;

import crude_engine.string;
import crude_engine.utility;

namespace crude_engine
{

inline void log()
{
  crude_engine::Log_Object(__FILE__, __LINE__);
}

inline void logInfo()
{
  crude_engine::Log_Object(__FILE__, __LINE__, Debug::Verbosity::Info);
}


inline void logWarning()
{
  crude_engine::Log_Object(__FILE__, __LINE__, Debug::Verbosity::Warning);
}


inline void logError()
{
  crude_engine::Log_Object(__FILE__, __LINE__, Debug::Verbosity::Error);
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