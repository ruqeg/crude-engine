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

import crude.core.logger;

namespace crude::core
{

Filesystem_Result readFile(const char* filename, span<char>& buffer)
{
  FILE *file = fopen(filename, "rb");
  if (file == nullptr)
  {
    core::logError(core::Debug::Channel::FileIO, "Failed to open file %s!", filename);
    return FILESYSTEM_RESULT_FAILED_OPEN;
  }
  fseek(file, 0, SEEK_END);
  size_t fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  if (buffer.size() < fileSize + 1)
  {
    fclose(file);
    core::logError(core::Debug::Channel::FileIO, "Failed to read file %s! Out of range!", filename);
    return FILESYSTEM_RESULT_OUT_OF_RANGE;
  }

  fread(buffer.data(), fileSize, 1, file);
  buffer[fileSize] = 0;
  fclose(file);

  return FILESYSTEM_RESULT_SUCCESS;
}

Filesystem_Result readFile(const char* filename, vector<char>& buffer)
{
  FILE* file = fopen(filename, "rb");
  if (file == nullptr)
  {
    core::logError(core::Debug::Channel::FileIO, "Failed to open file %s!", filename);
    return FILESYSTEM_RESULT_FAILED_OPEN;
  }
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

  return FILESYSTEM_RESULT_SUCCESS;
}

bool checkFileAccess(const char* filename)
{
  return (_access(filename, F_OK) == 0);
}

constexpr char getFileSeparator() 
{
#ifdef _WIN32
  return '\\';
#else
  return '/';
#endif
}

}