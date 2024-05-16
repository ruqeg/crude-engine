module;

#include <type_traits>

import crude_engine.core.alias;
import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_stack;
import crude_engine.core.std_containers_heap;

export module crude_engine.core.output_memory_steam;

export namespace crude_engine
{

class Output_Memory_System
{
public:
  Output_Memory_System();
  ~Output_Memory_System() = default;

public:
  Shared_Ptr<const vector<byte>> getBufferPtr() const;
  uint32 getLength() const;

  void write(const span<byte>& inData);
  
  template<class T>
  void write(T inData);

private:
  Shared_Ptr<vector<byte>>  m_buffer;
  uint32                    m_head;
};

template<class T>
void Output_Memory_System::write(T inData)
{
  static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic write only supports primitive data types");
  write(span(&inData, sizeof(inData)));
}

}