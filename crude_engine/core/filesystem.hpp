#pragma once

#include <core/data_structures/array_dynamic.hpp>
#include <core/data_structures/array_unsafe.hpp>

namespace crude_engine
{

class Filesystem 
{
public:
  enum Permission
  {
    PERMISSION_WRITE,
    PERMISSION_READ,
    PERMISSION_WRITE_READ
  };
  enum Result
  {
    RESULT_SUCCESS,
    RESULT_OUT_OF_RANGE,
    RESULT_FAILED_OPTN,
  };
public:
  static Filesystem& getInstance();

public:
  Result read(const char* filename, crude_engine::Array_Unsafe<char>& buffer);
  Result read(const char* filename, crude_engine::Array_Dynamic<char>& buffer);
  //void readAsync(char const *fileName);

  bool fileAccess(const char* filename);

  static inline constexpr char separator();

private:
  Filesystem() = default;
};

}