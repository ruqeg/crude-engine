#pragma once

#include <core/utility.hpp>

namespace crude_engine
{

class Memory_Utils
{
public:
  CRUDE_NODISCARD static void* allocate(std::size_t size) noexcept;
  static void free(void* memoryPtr) noexcept; 

  template<class T, class... Args >
  static CRUDE_NODISCARD T* constructAt(T* ptr, Args&&... args) noexcept
  {
    return ::new (reinterpret_cast<void*>(ptr)) T(Utility::forward<Args>(args)...);
  }

  template<class T>
  static void destructorAt(T* ptr) noexcept
  {
    ptr->~T();
  }
};

}