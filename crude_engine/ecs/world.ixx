module;

export module crude.ecs.world;

import crude.core.std_containers_stack;
import crude.core.std_containers_heap;
import crude.core.optional;
import crude.ecs.alias;
import crude.ecs.id_manager;
import crude.ecs.archetype;
import crude.ecs.component_register;
import crude.ecs.id_manager;

export namespace crude::ecs
{

template<class... Components>
class Query;

class Entity;

class World
{
private:
  struct Entity_Record
  {
    Entity_Record() = default;
    Entity_Record(Archetype_ID archetypeID)
      : archetypeID(archetypeID) {};
    Entity_Record(Archetype_ID archetypeID, core::uint64 row)
      : archetypeID(archetypeID), row(row) {};
    Archetype_ID      archetypeID;
    core::Optional<core::uint64>  row;
  };

  struct Archetype_Record
  {
    core::uint64 column;
  };

  struct Unsafe_Entity_Record
  {
    Unsafe_Entity_Record(Archetype* pArchetype, core::uint64 row)
      : pArchetype(pArchetype), row(row) {}
    Archetype*    pArchetype;
    core::uint64  row;
  };

private:
  using Archetype_Map = core::unordered_map<Archetype_ID, Archetype_Record>;

public:
  World();

public:
  Entity entity();

  template<class... Components>
  Query<Components...> query();

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

  void createArchetypeForEntity(Entity_ID entity, const core::set<Component_ID>& type);
  void assigneOrCreateArchetypeForEntity(Entity_ID entity, Component_ID component);
  void reassigneArchetypeForEntity(Entity_ID entity, Component_ID component);

  bool findArchetypeWithComponent(Component_ID component, const core::set<Component_ID>& type, Archetype_ID& dstArchetypeID);
  bool findArchetypeWithComponent(Component_ID component, Archetype_ID& dstArchetypeID);
  bool findArchetype(const core::set<Component_ID>& type, Archetype_ID& dstArchetypeID);

  void addArchetype(Archetype& archetype);
  void removeArchetype(Archetype& archetype);
  Archetype& getArchetype(const Archetype_ID archetypeID);
  const Archetype& getArchetype(const Archetype_ID archetypeID) const;

  void moveEntityComponentDataExceptAdded(core::uint32 skippedColumn, const Unsafe_Entity_Record& srcRecord, const Unsafe_Entity_Record& dstRecord);
  void moveEntityComponentDataExceptRemoved(core::uint32 skippedColumn, const Unsafe_Entity_Record& srcRecord, const Unsafe_Entity_Record& dstRecord);

private:
  ID_Manager m_entityIDsManager;
  ID_Manager m_systemIDsManager;
  ID_Manager m_archetypeIDsManager;

  core::unordered_map<Entity_ID, Entity_Record>     m_entityToRecord;
  core::unordered_map<Component_ID, Archetype_Map>  m_componentToArchetypeMap;
  core::shared_ptr<Component_Register>              m_componentRegister;
  core::vector<Archetype>                           m_archetypes;

  template<class ...Components>
  friend class Query;

  friend class Entity;
  friend class System;
  friend class Archetype;
};

template<class ...Components>
Query<Components...> World::query()
{
  core::uint64 index = 0u;
  core::array<Component_ID, sizeof...(Components)> components;
  ([&]() {
    components[index++] = core::CPP_Type<Components>::id();
    }(), ...);
  return Query<Components...>(this, components);
}

template<class Component>
void World::addComponent(Entity_ID entity)
{
  m_componentRegister->registerComponent<Component>();
  addComponent(entity, core::CPP_Type<Component>::id());
}

template<class Component>
void World::setComponent(Entity_ID entity, const Component& value)
{
  m_componentRegister->registerComponent<Component>();
  setComponent(entity, core::CPP_Type<Component>::id(), &value);
}

template<class Component>
void World::removeComponent(Entity_ID entity)
{
  removeComponent(entity, core::CPP_Type<Component>::id());
}

template<class Component>
bool World::hasComponent(Entity_ID entity) const
{
  return hasComponent(entity, core::CPP_Type<Component>::id());
}

template<class Component>
Component& World::getComponent(Entity_ID entity)
{
  void* value = getComponent(entity, core::CPP_Type<Component>::id());
  return *reinterpret_cast<Component*>(value);
}

template<class Component>
const Component& World::getComponent(Entity_ID entity) const
{
  void* value = getComponent(entity, core::CPP_Type<Component>::id());
  return *reinterpret_cast<Component*>(value);
}

}