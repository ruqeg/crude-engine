#pragma once

#include <core/ecs/alias.hpp>
#include <vector>
#include <unordered_map>
#include <set>
#include <queue>

// !TODO move to my stl

namespace crude_engine
{

class World;

struct Archetype_Edge
{
  Archetype_ID  addID;
  Archetype_ID  removeID;
};

class Archetype
{
public:
  struct Column
  {
    std::vector<uint8>   m_elements;
    Component_ID         m_component;
  };

public:
  Archetype(World* world, 
            Archetype_ID id, 
            const std::set<Component_ID>& type);

  void addEntity();
  void removeEntity();
  bool empty();
  uint64 newRow();
  void remove(uint64 row);
  void set(uint64 column, uint64 row, const void* value);
  void* get(uint64 column, uint64 row);
  const void* get(uint64 column, uint64 row) const;

  const std::set<Component_ID>& getType() const;
  Archetype_ID getID() const;

  uint64 getComponentsNum() const;

private:
  Archetype_ID                                 m_id;
  std::set<Component_ID>                       m_type;
  std::vector<Column>                          m_components;
  //std::unordered_map<Column, Archetype_Edge>   m_edges;
  uint64                                       m_entitesCapacity;
  uint64                                       m_entitesSize;
  std::queue<uint64>                           m_freeRows;
  uint64                                       m_entitiesNum;

  World* m_world;
};

}