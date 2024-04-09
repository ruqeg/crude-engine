#pragma once

#include <core/ecs/archetype.hpp>
#include <core/ecs/id_manager.hpp>
#include <queue>
#include <stack>
#include <unordered_map>
#include <core/utility.hpp>
#include <optional>
#include <core/debug/assert.hpp>
#include <core/ecs/component_register.hpp>

// !TODO move to my stl

namespace crude_engine
{

class Entity;

class World
{
private:
  struct Entity_Record
  {
    Archetype_ID           archetypeID;
    std::optional<uint64>  row;
  };

  struct Archetype_Record
  {
    uint64 column;
  };

private:
  using Archetype_Map = std::unordered_map<Archetype_ID, Archetype_Record>;

public:
  World();

public:
  Entity entity();
  void remove(Entity_ID id);

  template<class Component>
  void addComponent(Entity_ID entity);

  template<class Component>
  void setComponent(Entity_ID entity, const Component& value);

  template<class Component>
  void removeComponent(Entity_ID entity);

  template<class Component>
  bool hasComponent(Entity_ID entity) const;

  template<class Component>
  Component& getComponent(Entity_ID entity);

  template<class Component>
  const Component& getComponent(Entity_ID entity) const;

private:
  Entity_ID newID();

  void addComponent(Entity_ID entity, Component_ID component);
  void setComponent(Entity_ID entity, Component_ID component, const void* value);
  
  void* getComponent(Entity_ID entity, Component_ID component);
  const void* getComponent(Entity_ID entity, Component_ID component) const;
  
  void removeComponent(Entity_ID entity, Component_ID component);
  
  bool hasComponent(Entity_ID entity, Component_ID component) const;

  void createArchetypeForEntity(Entity_ID entity, const std::set<Component_ID>& type);
  void assigneOrCreateArchetypeForEntity(Entity_ID entity, Component_ID component);
  void reassigneArchetypeForEntity(Entity_ID entity, Component_ID component);

  bool findArchetype(Component_ID component, const std::set<Component_ID>& type, Archetype_ID& dstArchetypeID);
  bool findArchetype(Component_ID component, Archetype_ID& dstArchetypeID);

  void removeArchetype(Archetype& archetype);
  Archetype& getArchetype(const Archetype_ID archetypeID);
  const Archetype& getArchetype(const Archetype_ID archetypeID) const;

  uint64 moveComponentDataToArchetype(uint32 srcRow, uint32 srcSkippedColumn, Archetype& srcArchetype, Archetype& dstArchetype);

private:
  ID_Manager m_entityIDsManager;
  ID_Manager m_archetypeIDsManager;

  std::unordered_map<Entity_ID, Entity_Record>     m_entityToRecord;
  std::unordered_map<Component_ID, Archetype_Map>  m_componentToArchetypeMap;
  std::vector<Archetype>                           m_archetypes;
  Component_Register                               m_componentRegister;

  friend class Entity;
  friend class Archetype;
};

}

#include <core/ecs/world.inl>