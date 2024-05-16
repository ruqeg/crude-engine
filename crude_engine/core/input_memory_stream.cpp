#include <algorithm>

module crude_engine.core.input_memory_stream;

namespace crude_engine
{

Input_Memory_System::Input_Memory_System(Shared_Ptr<vector<byte>> inData)
  :
  m_buffer(inData),
  m_head(0u)
{}

uint32 Input_Memory_System::getRemainingDataSize() const
{
  return std::max(0ll, static_cast<int64>(m_buffer->size()) - m_head);
}

void Input_Memory_System::read(const span<byte>& outData)
{
  const uint32 readSize = std::clamp(static_cast<uint32>(outData.size_bytes()), 0u, getRemainingDataSize());
  memcpy(outData.data(), m_buffer->data() + m_head, readSize);
  m_head += readSize;
}

}