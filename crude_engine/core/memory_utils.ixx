module;

export module crude_engine.memory_utils;

import crude_engine.alias;
import crude_engine.utility;

export namespace crude_engine
{

class Memory_Utils
{
public:
  [[nodiscard]] static void* allocate(size_t size) noexcept;
  static void free(void* memoryPtr) noexcept; 
};

}