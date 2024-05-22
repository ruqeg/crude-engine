module;

#include <cmath>
#include <string.h>
#include <type_traits>
#include <algorithm>

export module crude.core.output_memory_bit_stream;

import crude.core.alias;
import crude.core.shared_ptr;
import crude.core.std_containers_stack;
import crude.core.std_containers_heap;

export namespace crude::core
{

class Output_Memory_Bit_Stream
{
public:
  Output_Memory_Bit_Stream();
  ~Output_Memory_Bit_Stream() = default;

public:
  void writeBits(uint8 inData, uint64 inBitCount);
  void writeBits(const void* inData, uint64 inBitCount);

  Shared_Ptr<vector<byte>> getBufferPtr() const;
  uint32 getBitLength() const;
  uint32 getByteLength() const;

  void writeBytes(const void* inData, uint64 inByteCount);

  template<class T>
  void write(T inData, uint64 inBitCount = sizeof(T) * 8)
  {
    static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic write only supports primitive data types");
    writeBits(&inData, inBitCount);
  }

private:
  void reallocBuffer(uint32 inNewBiyCapacity);

private:
  Shared_Ptr<vector<byte>>  m_buffer;
  uint32                    m_bitHead;
  uint32                    m_bitCapacity;
};

Output_Memory_Bit_Stream::Output_Memory_Bit_Stream()
  :
  m_bitHead(0u),
  m_bitCapacity(0u),
  m_buffer(makeShared<vector<byte>>())
{
  reallocBuffer(256);
}

void Output_Memory_Bit_Stream::writeBits(uint8 inData, uint64 inBitCount)
{
  uint32 nextBidHead = m_bitHead + static_cast<uint32>(inBitCount);
  if (nextBidHead > m_bitCapacity)
  {
    reallocBuffer(std::max(m_bitCapacity * 2, nextBidHead));
  }

  uint32 byteOffset = m_bitHead >> 3;
  uint32 bitOffset = m_bitHead & 0x7;

  uint8 currentMask = ~(0xff << bitOffset);
  m_buffer->at(byteOffset) = (m_buffer->at(byteOffset) & currentMask) | (inData << bitOffset);

  uint32 bitsFreeThisByte = 8 - bitOffset;

  if (bitsFreeThisByte < inBitCount)
  {
    m_buffer->at(byteOffset + 1) = inData >> bitsFreeThisByte;
  }
  
  m_bitHead = nextBidHead;
}

void Output_Memory_Bit_Stream::writeBits(const void* inData, uint64 inBitCount)
{
  const byte* srcByte = static_cast<const byte*>(inData);

  while (inBitCount > 0)
  {
    writeBits(*srcByte, 8);
    ++srcByte;
    inBitCount -= 8;
  }

  if (inBitCount > 0)
  {
    writeBits(*srcByte, inBitCount);
  }
}

Shared_Ptr<vector<byte>> Output_Memory_Bit_Stream::getBufferPtr() const
{
  return m_buffer;
}

uint32 Output_Memory_Bit_Stream::getBitLength() const
{
  return m_bitHead;
}

uint32 Output_Memory_Bit_Stream::getByteLength() const
{
  return (m_bitHead + 7) >> 3;
}

void Output_Memory_Bit_Stream::writeBytes(const void* inData, uint64 inByteCount)
{
  writeBits(inData, inByteCount << 3);
}

void Output_Memory_Bit_Stream::reallocBuffer(uint32 inNewBiyCapacity)
{
  m_bitCapacity = inNewBiyCapacity;
  m_buffer->resize((m_bitCapacity + 7) >> 3);
}

}