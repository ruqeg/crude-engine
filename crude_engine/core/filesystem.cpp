#include <core/filesystem.hpp>

#ifdef _WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

namespace crude_engine
{

Filesystem& Filesystem::getInstance()
{
  Filesystem instance;
  return instance;
}

Filesystem::Result Filesystem::read(const char* filename, crude_engine::Array_Unsafe<char>& buffer)
{
  FILE *file = fopen(filename, "rb");
  fseek(file, 0, SEEK_END);
  std::size_t fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  if (buffer.size() < fileSize + 1)
  {
    fclose(file);
    return RESULT_OUT_OF_RANGE;
  }

  fread(buffer.data(), fileSize, 1, file);
  buffer[fileSize] = 0;
  fclose(file);

  return RESULT_SUCCESS;
}

Filesystem::Result Filesystem::read(const char* filename, crude_engine::Array_Dynamic<char>& buffer)
{
  FILE* file = fopen(filename, "rb");
  fseek(file, 0, SEEK_END);
  std::size_t fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  if (buffer.size() < fileSize + 1)
  {
    buffer.resize(fileSize + 1);
  }

  fread(buffer.data(), fileSize, 1, file);
  buffer[fileSize] = 0;
  fclose(file);

  return RESULT_SUCCESS;
}

bool Filesystem::fileAccess(const char* filename)
{
  return (access(filename, F_OK) == 0);
}

constexpr char Filesystem::separator() {
#ifdef _WIN32
  return '\\';
#else
  return '/';
#endif
}

}