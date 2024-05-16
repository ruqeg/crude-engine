#include <string.h>
#include <type_traits>
#include <algorithm>

module crude_engine.core.output_memory_steam;

namespace crude_engine
{

Output_Memory_System::Output_Memory_System()
  :
  m_buffer(makeShared<vector<byte>>()),
  m_head(0u)
{}

Shared_Ptr<const vector<byte>> Output_Memory_System::getBufferPtr() const
{
  return m_buffer;
}

uint32 Output_Memory_System::getLength() const
{
  return m_head;
}

void Output_Memory_System::write(const span<byte>& inData)
{
  const uint32 capacity = m_buffer->size();
  const uint32 resultHead = m_head + static_cast<uint32>(inData.size_bytes());
  if (resultHead > capacity)
  {
    m_buffer->resize(std::max(capacity * 2, resultHead));
  }

  memcpy(m_buffer->data() + m_head, inData.data(), inData.size_bytes());

  m_head = resultHead;
}

}