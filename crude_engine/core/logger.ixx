module;

#include <utility>
#include <Windows.h>

export module crude_engine.core.logger;

export import crude_engine.core.debug;
export import crude_engine.core.alias;

import crude_engine.core.string;
import crude_engine.core.utility;

export namespace crude_engine
{

class Log_Object;

template<typename... Args>
inline void log(const Debug::Channel channel, const char* format, Args&&... args);
template<typename... Args>
inline void logInfo(const Debug::Channel channel, const char* format, Args&&... args);
template<typename... Args>
inline void logWarning(const Debug::Channel channel, const char* format, Args&&... args);
template<typename... Args>
inline void logError(const Debug::Channel channel, const char* format, Args&&... args);

class Logger
{
public:
  template<typename... Args>
  static void debugPrintF(const char*             filename,
                          const int32             line,
                          const Debug::Channel    channel,
                          const Debug::Verbosity  verbosity,
                          const char*             format,
                          Args&&...               args) noexcept;

private:
  static void outputStr(const char* str);

private:
  Logger() = default;
};


class Log_Object
{
public:
  Log_Object(const char* filename, const int32 line);
  Log_Object(const char* filename, const int32 line, Debug::Verbosity verbosity);

  template<typename... Args>
  void operator()(const Debug::Channel  channel,
    const char* format,
    Args&&...             args) const noexcept;

private:
  const char* m_filename;
  int32             m_line;
  Debug::Verbosity  m_verbosity;
};

template<typename... Args>
inline void log(const Debug::Channel channel, const char* format, Args&&... args)
{
  Log_Object(__FILE__, __LINE__)(channel, format, std::forward<Args>(args)...);
}

template<typename... Args>
inline void logInfo(const Debug::Channel channel, const char* format, Args&&... args)
{
  Log_Object(__FILE__, __LINE__, Debug::Verbosity::Info)(channel, format, std::forward<Args>(args)...);
}

template<typename... Args>
inline void logWarning(const Debug::Channel channel, const char* format, Args&&... args)
{
  Log_Object(__FILE__, __LINE__, Debug::Verbosity::Warning)(channel, format, std::forward<Args>(args)...);
}

template<typename... Args>
inline void logError(const Debug::Channel channel, const char* format, Args&&... args)
{
  Log_Object(__FILE__, __LINE__, Debug::Verbosity::Error)(channel, format, std::forward<Args>(args)...);
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
void Logger::debugPrintF(const char* filename,
                         const int32             line,
                         const Debug::Channel    channel,
                         const Debug::Verbosity  verbosity,
                         const char* format,
                         Args&&...               args) noexcept
{
  char messageBuffer[1024] = {};
  char formatBuffer[1024] = {};

  strncat(formatBuffer, sizeof(formatBuffer), "[channel: %s][verbosity: %s][filename: %s][line: %i] =>\n\t");
  strncat(formatBuffer, sizeof(formatBuffer), format);
  strncat(formatBuffer, sizeof(formatBuffer), "\n");
  snprintf(messageBuffer, sizeof(messageBuffer), formatBuffer, Debug::toString(channel), Debug::toString(verbosity), filename, line, std::forward<Args>(args)...);
  outputStr(messageBuffer);
}

template<typename ...Args>
void Log_Object::operator()(const Debug::Channel  channel,
  const char* format,
  Args&&...             args) const noexcept
{
  Logger::debugPrintF(m_filename, m_line, channel, m_verbosity, format, std::forward<Args>(args)...);
}

}