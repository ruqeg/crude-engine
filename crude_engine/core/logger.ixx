module;

export module crude_engine.logger;

export import crude_engine.debug;
import crude_engine.alias;

export namespace crude_engine
{

inline void log();
inline void logInfo();
inline void logWarning();
inline void logError();

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