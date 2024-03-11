#pragma once

#include <core/alias.hpp>
#include <vector>
#include <unordered_map>

namespace crude_engine
{

using Component_ID = uint64;
using Archetype_ID = uint64;

struct Column
{
  void*   elements;     // buffer with component data
  uint64  element_size; // size of a single element
  uint64  count;        // number of elements
};

struct Archetype;

struct Archetype_Edge
{
  Archetype*  pAdd;
  Archetype*  pRemove;
};

class Archetype
{
public:
  std::vector<Component_ID>                          m_type;
  Archetype_ID                                       m_id;
  std::vector<Column>                                m_components; // one vector for each component
  std::unordered_map<Component_ID, Archetype_Edge&>  m_edges;
};

}