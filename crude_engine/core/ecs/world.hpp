#pragma once

#include <core/ecs/archetype.hpp>
#include <core/ecs/id_manager.hpp>
#include <queue>
#include <stack>
#include <unordered_map>
#include <core/utility.hpp>
#include <optional>
#include <core/debug/assert.hpp>

namespace crude_engine
{

using Entity_ID     = ID;
using Component_ID  = ID;

struct Record
{
  Archetype_ID           archetypeID;
  std::optional<uint64>  row;
};

struct Archetype_Record
{
  uint64 column;
};

using Move_Component_Functinon = void (*)(void* dst, void* src);
using Copy_Component_Functinon = void (*)(void* dst, const void* src);

class Entity;

class World
{
public:
  using Archetype_Map = std::unordered_map<Archetype_ID, Archetype_Record>;

public:
  World();

  Entity entity();

private:
  Entity_ID newID();

private:
  template<class Component>
  void addComponent(Entity_ID entity);

  template<class Component>
  void setComponent(Entity_ID entity, const Component& value);

  template<class Component>
  void removeComponent(Entity_ID entity);

  template<class Component>
  bool hasComponent(Entity_ID entity) const;

  void addComponent(Entity_ID entity, Component_ID component);
  void setComponent(Entity_ID entity, Component_ID component, const void* value);
  void* getComponent(Entity_ID entity, Component_ID component);
  void removeComponent(Entity_ID entity, Component_ID component);
  bool hasComponent(Entity_ID entity, Component_ID component) const;
  
  template<class Component>
  Component& getComponent(Entity_ID entity);

  template<class Component>
  const Component& getComponent(Entity_ID entity) const;

  void remove(Entity_ID id);

private:
  void createArchetypeForEntity(Entity_ID entity, const std::set<Component_ID>& type);
  void assigneOrCreateArchetypeForEntity(Entity_ID entity, Component_ID component);
  void reassigneArchetypeForEntity(Entity_ID entity, Component_ID component);

  bool findArchetypeWithComponent(Component_ID component, const std::set<Component_ID>& type, Archetype_ID& archetypeID);
  bool findArchetypeWithComponent(Component_ID component, Archetype_ID& archetypeID);

  void removeArchetype(Archetype& archetype);
  void addArchetypeToArray(const Archetype& archetype);
  Archetype& getArchetypeFromID(const Archetype_ID archetypeID);

private:
  ID_Manager m_entityIDsManager;
  ID_Manager m_archetypeIDsManager;

  std::unordered_map<Entity_ID, Record>            m_entityToRecord;
  std::unordered_map<Component_ID, Archetype_Map>  m_componentToArchetypeRecord;
  std::vector<Archetype>                           m_archetypes;
  std::unordered_map<Component_ID, Move_Component_Functinon> m_componentToMoveFunc;
  std::unordered_map<Component_ID, size_t>                   m_componentToElementSize;
  std::unordered_map<Component_ID, Copy_Component_Functinon> m_componentToCopyFunc;

  friend class Entity;
};

}

#include <core/ecs/world.inl>