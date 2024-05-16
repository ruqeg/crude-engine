module;

#include <type_traits>
#include <algorithm>

import crude_engine.core.alias;
import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_stack;
import crude_engine.core.std_containers_heap;

export module crude_engine.core.input_memory_stream;

export namespace crude_engine
{

class Input_Memory_System
{
public:
  Input_Memory_System(Shared_Ptr<vector<byte>> inData);
  ~Input_Memory_System() = default;

public:
  uint32 getRemainingDataSize() const;

  void read(byte* outData, uint32 inByteCount);

  template<class T>
  void read(T& outData);

private:
  Shared_Ptr<vector<byte>>  m_buffer;
  uint32                    m_head;
};

Input_Memory_System::Input_Memory_System(Shared_Ptr<vector<byte>> inData)
  :
  m_buffer(inData),
  m_head(0u)
{}

uint32 Input_Memory_System::getRemainingDataSize() const
{
  return std::max(0ll, static_cast<int64>(m_buffer->size()) - m_head);
}

void Input_Memory_System::read(byte* outData, uint32 inByteCount)
{
  const uint32 readSize = std::clamp(inByteCount, 0u, getRemainingDataSize());
  memcpy(outData, m_buffer->data() + m_head, readSize);
  m_head += readSize;
}

template<class T>
void Input_Memory_System::read(T& outData)
{
  static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic write only supports primitive data types");
  read(reinterpret_cast<byte*>(&outData), sizeof(outData));
}

}