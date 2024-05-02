#pragma once

#include <core/string.hpp>
#include <core/debug.hpp>
#include <core/utility.hpp>


#define CRUDE_LOG \
  crude_engine::Log_Object(__FILENAME__, __LINE__)

#define CRUDE_LOG_INFO \
  crude_engine::Log_Object(__FILENAME__, __LINE__, crude_engine::Debug::Verbosity::Info)

#define CRUDE_LOG_WARNING \
  crude_engine::Log_Object(__FILENAME__, __LINE__, crude_engine::Debug::Verbosity::Warning)

#define CRUDE_LOG_ERROR \
  crude_engine::Log_Object(__FILENAME__, __LINE__, crude_engine::Debug::Verbosity::Error)


namespace crude_engine
{

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
  void operator()(const Debug::Channel    channel,
                  const char*             format,
                  Args&&...               args) const noexcept;

private:
  const char*       m_filename;
  int32             m_line;
  Debug::Verbosity  m_verbosity;
};

}

#include <core/logger.inl>