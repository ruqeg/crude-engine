#pragma once

#include <ecs/alias.hpp>
#include <core/std_containers.hpp>

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
  queue<ID> m_freeIDs;
  uint64 m_newFreeIndex;
};

}