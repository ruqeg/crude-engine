#pragma once

#include <core/ecs/archetype.hpp>
#include <core/ecs/id_manager.hpp>
#include <queue>
#include <stack>
#include <unordered_map>

namespace crude_engine
{

using Entity_ID     = ID;
using Component_ID  = ID;

struct Record
{
  Archetype_ID  archetypeID;
  uint64        row;
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

  bool findArchetypeWithComponent(Component_ID component, const std::set<Component_ID>& type, Archetype_ID& archetypeID);
  bool findArchetypeWithComponent(Component_ID component, Archetype_ID& archetypeID);

  void addArchetypeToArray(const Archetype& archetype);
  Archetype& getArchetypeFromID(const Archetype_ID archetypeID);

private:
  ID_Manager m_entityIDsManager;
  ID_Manager m_archetypeIDsManager;

  std::unordered_map<Archetype_ID, size_t>         m_archetypeToIndex;
  std::unordered_map<Entity_ID, Record>            m_entityToRecord;
  std::unordered_map<Component_ID, Archetype_Map>  m_componentToArchetypeRecord;
  std::vector<Archetype>                           m_archetypes;
};

}