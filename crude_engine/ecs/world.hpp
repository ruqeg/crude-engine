#pragma once

#include <ecs/archetype.hpp>
#include <ecs/id_manager.hpp>
#include <queue>
#include <stack>
#include <unordered_map>
#include <core/utility.hpp>
#include <optional>
#include <core/assert.hpp>
#include <ecs/component_register.hpp>
#include <ecs/query.hpp>
#include <vector>

// !TODO move to my stl

namespace crude_engine
{

class Entity;
class World
{
private:
  struct Entity_Record
  {
    Entity_Record() = default;
    Entity_Record(Archetype_ID archetypeID) 
      : archetypeID(archetypeID) {};
    Entity_Record(Archetype_ID archetypeID, uint64 row) 
      : archetypeID(archetypeID), row(row) {};
    Archetype_ID           archetypeID;
    std::optional<uint64>  row;
  };

  struct Archetype_Record
  {
    uint64 column;
  };

  struct Unsafe_Entity_Record
  {
    Unsafe_Entity_Record(Archetype* pArchetype, uint64 row)
      : pArchetype(pArchetype), row(row) {}
    Archetype*  pArchetype;
    uint64      row;
  };

private:
  using Archetype_Map = std::unordered_map<Archetype_ID, Archetype_Record>;

public:
  World();

public:
  Entity entity();

  //template<class... Components>
  //Query query() const;

  void remove(Entity_ID entity);

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
  Entity_ID newEntityID();
  Entity_ID newSystemID();

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

  void addArchetype(Archetype&& archetype);
  void removeArchetype(Archetype& archetype);
  Archetype& getArchetype(const Archetype_ID archetypeID);
  const Archetype& getArchetype(const Archetype_ID archetypeID) const;

  void moveEntityComponentDataExceptAdded(uint32 skippedColumn, const Unsafe_Entity_Record& srcRecord, const Unsafe_Entity_Record& dstRecord);
  void moveEntityComponentDataExceptRemoved(uint32 skippedColumn, const Unsafe_Entity_Record& srcRecord, const Unsafe_Entity_Record& dstRecord);

private:
  ID_Manager m_entityIDsManager;
  ID_Manager m_systemIDsManager;
  ID_Manager m_archetypeIDsManager;

  std::unordered_map<Entity_ID, Entity_Record>     m_entityToRecord;
  std::unordered_map<Component_ID, Archetype_Map>  m_componentToArchetypeMap;
  std::vector<Archetype>                           m_archetypes;
  std::shared_ptr<Component_Register>              m_componentRegister;

  template<class ...Components>
  friend class Query;

  friend class Entity;
  friend class System;
  friend class Archetype;
};

}

#include <ecs/world.inl>