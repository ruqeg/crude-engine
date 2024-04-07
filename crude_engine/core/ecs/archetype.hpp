#pragma once

#include <core/alias.hpp>
#include <vector>
#include <unordered_map>
#include <set>
#include <queue>

namespace crude_engine
{

using Component_ID = uint64;
using Archetype_ID = uint64;

struct Column
{
  std::vector<char8>   elements;     // buffer with component data
  uint64               elementsNum = 0u;
  Component_ID         component;
};

struct Archetype;

struct Archetype_Edge
{
  Archetype_ID  addID;
  Archetype_ID  removeID;
};

class Archetype
{
public:
  std::set<Component_ID>                             m_type;
  Archetype_ID                                       m_id;
  std::vector<Column>                                m_components; // one vector for each component
  std::unordered_map<Component_ID, Archetype_Edge&>  m_edges;
  size_t                                             m_entitiesNum;
  std::queue<size_t>                                 m_freeRow;
};

}