module crude.core.string;

namespace crude::core
{

size_t strlen(char* const buffer) noexcept
{
  size_t len = 0u;

  while (buffer[len] != '\0')
  {
    len++;
  }

  return len;
}

char* strncat(char* const buffer, const size_t bufferCount, const char* const source) noexcept
{
  size_t bufferLen = strlen(buffer);
  size_t i;

  for (i = 0u; (i < bufferCount) && (source[i] != '\0'); i++)
  {
    buffer[bufferLen + i] = source[i];
  }
  buffer[bufferLen + i] = '\0';

  return buffer;
}

}
