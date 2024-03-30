#include <core/ecs/world.hpp>

namespace crude_engine
{

template<class Component>
void World::addComponent(Entity_ID entity)
{
  addComponent(entity, CPP_Type<Component>::id(), sizeof(Component));
}

template<class Component>
void World::setComponent(Entity_ID entity, const Component& value)
{
  const Component_ID componentID = CPP_Type<Component>::id();

  if (!hasComponent<Component>(entity))
  {
    // !TODO
    CRUDE_ASSERT(false && "TODO");
    return;
  }

  Record& entityRec = m_entityToRecord[entity];
  Archetype_Map& componentRec = m_componentToArchetypeRecord[componentID];
  Archetype& archetype = getArchetypeFromID(entityRec.archetypeID);
  Column& mcolumn = archetype.m_components[componentRec[entityRec.archetypeID].column];
  if (!entityRec.row.has_value())
  {
    for (auto& column : archetype.m_components)
    {
      column.elements.resize(column.elements.size() + column.elementSize);
    }

    entityRec.row = static_cast<int64>(mcolumn.elements.size() / mcolumn.elementSize) - 1;
  }
  Component* currentValue = reinterpret_cast<Component*>(mcolumn.elements.data() + (entityRec.row.value() * mcolumn.elementSize));
  *currentValue = value;
}

template<class Component>
bool World::hasComponent(Entity_ID entity) const
{
  return hasComponent(entity, CPP_Type<Component>::id());
}

template<class Component>
Component& World::getComponent(Entity_ID entity)
{
  const Component_ID componentID = CPP_Type<Component>::id();

  if (!hasComponent<Component>(entity))
  {
    // !TODO
    CRUDE_ASSERT(false && "TODO");
  }

  Record& entityRec = m_entityToRecord[entity];
  Archetype_Map& componentRec = m_componentToArchetypeRecord[componentID];
  Archetype& archetype = getArchetypeFromID(entityRec.archetypeID);
  Column& mcolumn = archetype.m_components[componentRec[entityRec.archetypeID].column];
  Component* value = reinterpret_cast<Component*>(mcolumn.elements.data() + (entityRec.row.value() * mcolumn.elementSize));
  return *value;
}

template<class Component>
const Component& World::getComponent(Entity_ID entity) const
{
  const Component_ID componentID = CPP_Type<Component>::id();

  if (!hasComponent<Component>(entity))
  {
    // !TODO
    CRUDE_ASSERT(false && "TODO");
  }

  Record& entityRec = m_entityToRecord[entity];
  Archetype_Map& componentRec = m_componentToArchetypeRecord[componentID];
  Archetype& archetype = getArchetypeFromID(entityRec.archetypeID);
  Column& mcolumn = archetype.m_components[componentRec[entityRec.archetypeID].column];
  Component* value = reinterpret_cast<Component*>(mcolumn.elements.data() + (entityRec.row.value() * mcolumn.elementSize));
  return *value;
}

}