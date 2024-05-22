module;

#include <cmath>
#include <string.h>
#include <type_traits>
#include <algorithm>

export module crude.core.input_memory_bit_stream;

import crude.core.alias;
import crude.core.shared_ptr;
import crude.core.std_containers_stack;
import crude.core.std_containers_heap;

export namespace crude::core
{

class Input_Memory_Bit_Stream
{
public:
  Input_Memory_Bit_Stream(Shared_Ptr<vector<byte>> inData);
  ~Input_Memory_Bit_Stream() = default;

public:
  void readBits(uint8& outData, uint64 inBitCount);
  void readBits(void* outData, uint64 inBitCount);

  Shared_Ptr<vector<byte>> getBufferPtr() const;
  uint32 getBitLength() const;
  uint32 getByteLength() const;

  void readBytes(void* outData, uint64 inByteCount);

  template<class T>
  void read(T& outData, uint64 inBitCount = sizeof(T) * 8)
  {
    static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic write only supports primitive data types");
    readBits(&outData, inBitCount);
  }

private:
  Shared_Ptr<vector<byte>>  m_buffer;
  uint32                    m_bitHead;
  uint32                    m_bitCapacity;
};

Input_Memory_Bit_Stream::Input_Memory_Bit_Stream(Shared_Ptr<vector<byte>> inData)
  :
  m_bitHead(0u),
  m_bitCapacity(inData->size() << 3),
  m_buffer(inData)
{}

void Input_Memory_Bit_Stream::readBits(uint8& outData, uint64 inBitCount)
{
  uint32 byteOffset = m_bitHead >> 3;
  uint32 bitOffset = m_bitHead & 0x7;
  
  uint32 bitInFirstByte = 8 - bitOffset;

  if (inBitCount > bitInFirstByte)
  {
    uint8 currentMask = (0xff >> bitOffset);
    outData = (currentMask & m_buffer->at(byteOffset)) << bitOffset;
    currentMask = ~(0xff >> (inBitCount - bitInFirstByte));
    outData |= (currentMask & m_buffer->at(byteOffset + 1u)) >> bitInFirstByte;
  }
  else
  {
    uint8 currentMask = (0xff >> bitOffset) & (0xff << (bitInFirstByte - inBitCount));
    outData = (currentMask & m_buffer->at(byteOffset)) << bitOffset;
  }

  m_bitHead += inBitCount;
}

void Input_Memory_Bit_Stream::readBits(void* outData, uint64 inBitCount)
{
  byte* outByte = static_cast<byte*>(outData);

  while (inBitCount > 0)
  {
    readBits(*outByte, 8);
    ++outByte;
    inBitCount -= 8;
  }

  if (inBitCount > 0)
  {
    readBits(*outByte, inBitCount);
  }
}

Shared_Ptr<vector<byte>> Input_Memory_Bit_Stream::getBufferPtr() const
{
  return m_buffer;
}

uint32 Input_Memory_Bit_Stream::getBitLength() const
{
  return m_bitHead;
}

uint32 Input_Memory_Bit_Stream::getByteLength() const
{
  return (m_bitHead + 7) >> 3;
}

void Input_Memory_Bit_Stream::readBytes(void* outData, uint64 inByteCount)
{
  readBits(outData, inByteCount << 3);
}

}