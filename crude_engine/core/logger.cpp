#include <core/logger.hpp>
#include <Windows.h>

namespace crude_engine
{

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

}