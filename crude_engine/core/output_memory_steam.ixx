module;

#include <string.h>
#include <type_traits>
#include <algorithm>

import crude.core.alias;
import crude.core.std_containers_stack;
import crude.core.std_containers_heap;

export module crude.core.output_memory_stream;

export namespace crude::core
{

class Output_Memory_Stream
{
public:
  Output_Memory_Stream();
  ~Output_Memory_Stream() = default;

public:
  shared_ptr<vector<byte>> getBufferPtr() const;
  uint32 getLength() const;

  void write(const void* inData, uint32 inByteCount);

  template<class T>
  void write(const T& inData);

private:
  shared_ptr<vector<byte>>  m_buffer;
  uint32                    m_head;
};

Output_Memory_Stream::Output_Memory_Stream()
  :
  m_buffer(allocateShared<vector<byte>>()),
  m_head(0u)
{}

shared_ptr<vector<byte>> Output_Memory_Stream::getBufferPtr() const
{
  return m_buffer;
}

uint32 Output_Memory_Stream::getLength() const
{
  return m_head;
}

void Output_Memory_Stream::write(const void* inData, uint32 inByteCount)
{
  const uint32 capacity = m_buffer->size();
  const uint32 resultHead = m_head + static_cast<uint32>(inByteCount);
  if (resultHead > capacity)
  {
    m_buffer->resize(std::max(capacity * 2, resultHead));
  }

  memcpy(m_buffer->data() + m_head, inData, inByteCount);

  m_head = resultHead;
}

template<class T>
void Output_Memory_Stream::write(const T& inData)
{
  static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic write only supports primitive data types");
  write(&inData, sizeof(inData));
}

}