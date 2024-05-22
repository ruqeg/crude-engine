#include <vector>

#ifdef _WIN32
#include <io.h.>
#include <stdio.h.>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

module crude.core.filesystem;

namespace crude::core
{

Filesystem& Filesystem::getInstance()
{
  Filesystem instance;
  return instance;
}

Filesystem::Result Filesystem::read(const char* filename, span<char>& buffer)
{
  FILE *file = fopen(filename, "rb");
  fseek(file, 0, SEEK_END);
  size_t fileSize = ftell(file);
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

Filesystem::Result Filesystem::read(const char* filename, vector<char>& buffer)
{
  FILE* file = fopen(filename, "rb");
  fseek(file, 0, SEEK_END);
  size_t fileSize = ftell(file);
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
  return (_access(filename, F_OK) == 0);
}

constexpr char Filesystem::separator() {
#ifdef _WIN32
  return '\\';
#else
  return '/';
#endif
}

}