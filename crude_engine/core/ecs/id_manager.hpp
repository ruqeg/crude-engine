#pragma once

#include <core/alias.hpp>
#include <queue>

namespace crude_engine
{

using ID            = uint64;
using ID_Generation = uint16;
using ID_Index      = uint64;

class ID_Manager
{
public:
  ID_Manager();
  
public:
  ID generate();
  void destroy(ID id);

public:
  static ID format(ID_Index index, ID_Generation generation);
  static ID_Generation getGeneration(ID id);
  static ID_Index getIndex(ID id);

private:
  std::queue<ID> m_freeIDs;
  uint64 m_newFreeIndex;
};

}