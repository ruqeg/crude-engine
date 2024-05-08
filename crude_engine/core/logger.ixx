module;

export module crude_engine.core.logger;

export import crude_engine.core.debug;
export import crude_engine.core.alias;

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
                  const char*           format,
                  Args&&...             args) const noexcept;

private:
  const char*       m_filename;
  int32             m_line;
  Debug::Verbosity  m_verbosity;
};

}