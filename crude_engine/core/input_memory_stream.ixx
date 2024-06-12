module;

#include <type_traits>
#include <algorithm>

import crude.core.alias;
import crude.core.std_containers_stack;
import crude.core.std_containers_heap;

export module crude.core.output_memory_bit_stream;

export namespace crude::core
{

class Input_Memory_Stream
{
public:
  Input_Memory_Stream(shared_ptr<vector<byte>> inData);
  ~Input_Memory_Stream() = default;

public:
  uint32 getRemainingDataSize() const;

  void read(void* outData, uint32 inByteCount);

  template<class T>
  void read(T& outData);

private:
  shared_ptr<vector<byte>>  m_buffer;
  uint32                    m_head;
};

Input_Memory_Stream::Input_Memory_Stream(shared_ptr<vector<byte>> inData)
  :
  m_buffer(inData),
  m_head(0u)
{}

uint32 Input_Memory_Stream::getRemainingDataSize() const
{
  return std::max(0ll, static_cast<int64>(m_buffer->size()) - m_head);
}

void Input_Memory_Stream::read(void* outData, uint32 inByteCount)
{
  const uint32 readSize = std::clamp(inByteCount, 0u, getRemainingDataSize());
  memcpy(outData, m_buffer->data() + m_head, readSize);
  m_head += readSize;
}

template<class T>
void Input_Memory_Stream::read(T& outData)
{
  static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic write only supports primitive data types");
  read(&outData, sizeof(outData));
}

}