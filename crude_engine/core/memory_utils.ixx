module;

export module crude_engine.core.memory_utils;

import crude_engine.core.alias;
import crude_engine.core.utility;

export namespace crude_engine
{

class Memory_Utils
{
public:
  [[nodiscard]] static void* allocate(size_t size) noexcept;
  static void free(void* memoryPtr) noexcept; 
};

}