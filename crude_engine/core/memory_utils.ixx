module;

export module crude.core.memory_utils;

import crude.core.alias;
import crude.core.utility;

export namespace crude::core
{

class Memory_Utils
{
public:
  [[nodiscard]] static void* allocate(size_t size) noexcept;
  static void free(void* memoryPtr) noexcept; 
};

}