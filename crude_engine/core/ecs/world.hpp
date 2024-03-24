#pragma once

#include <core/ecs/archetype.hpp>
#include <queue>
#include <stack>
#include <unordered_map>

namespace crude_engine
{

using Entity_ID     = uint64;
using Component_ID  = uint64;

struct Record
{
  Archetype*  pArchetype;
  uint64      row;
};

struct Archetype_Record
{
  uint64 column;
};

class Entity;

class World
{
public:
  using Archetype_Map = std::unordered_map<Archetype_ID, Archetype_Record>;

public:
  World();

  Entity entity();
  Entity_ID newID();

  void addComponent(Entity_ID entity, Component_ID component);
  bool hasComponent(Entity_ID entity, Component_ID component) const;

  void remove(Entity_ID id);

private:
  void createArchetypeForEntity(Entity_ID entity, const std::set<Component_ID>& type);
  void assigneOrCreateArchetypeForEntity(Entity_ID entity, Component_ID component);
  void reassigneArchetypeForEntity(Entity_ID entity, Component_ID component);

  bool findArchetypeWithComponent(Component_ID component, const std::set<Component_ID>& type, Archetype_ID& dstAcrhetypeID);

private:
  std::queue<Entity_ID>  m_freeEntityIDs;
  uint64                 m_newFreeEntityID;
  uint64                 m_newFreeArchetypeID;

  std::unordered_map<Entity_ID, Record>            m_entityToRecord;
  std::unordered_map<Component_ID, Archetype_Map>  m_componentToArchetypeRecord;
  std::vector<Archetype>                           m_archetypes;
  std::queue<Archetype_ID>                         m_freeArchetypeIDs;
};

}