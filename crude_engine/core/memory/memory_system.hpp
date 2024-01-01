#pragma once

#include <core/memory/memory_manager.hpp>

namespace crude_engine
{

class Memory_System
{
public:
  template<class T, typename... Args>
  static CRUDE_NODISCARD T* mnewFRBTA(std::size_t n, Args&&... args) noexcept;

  template<class T>
  static void mdeleteFRBTA(T* ptr) noexcept;
};

}  // namespace crude_engine

#include <core/memory/memory_system.inl>