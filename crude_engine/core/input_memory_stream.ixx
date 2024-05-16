module;

#include <type_traits>

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

  void read(const span<byte>& outData);

  template<class T>
  void read(T& outData);

private:
  Shared_Ptr<vector<byte>>  m_buffer;
  uint32                    m_head;
};

template<class T>
void Input_Memory_System::read(T& outData)
{
  static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic write only supports primitive data types");
  read(span(&outData, sizeof(outData)));
}

}