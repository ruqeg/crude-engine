#pragma once

#include <core/ecs/alias.hpp>
#include <queue>

namespace crude_engine
{

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