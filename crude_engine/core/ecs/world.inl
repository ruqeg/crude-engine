#include <core/ecs/world.hpp>

namespace crude_engine
{

template<class Component>
void World::addComponent(Entity_ID entity)
{
  const std::size_t componentID = CPP_Type<Component>::id();
  m_componentToMoveFunc[componentID]     = Component::Move;
  m_componentToElementSize[componentID]  = sizeof(Component);
  m_componentToCopyFunc[componentID]     = Component::Copy;
  addComponent(entity, componentID);
}

template<class Component>
void World::setComponent(Entity_ID entity, const Component& value)
{
  const std::size_t componentID = CPP_Type<Component>::id();
  m_componentToMoveFunc[componentID]    = Component::Move;
  m_componentToElementSize[componentID] = sizeof(Component);
  m_componentToCopyFunc[componentID]    = Component::Copy;
  setComponent(entity, componentID, &value);
}

template<class Component>
void World::removeComponent(Entity_ID entity)
{
  removeComponent(entity, CPP_Type<Component>::id());
}

template<class Component>
bool World::hasComponent(Entity_ID entity) const
{
  return hasComponent(entity, CPP_Type<Component>::id());
}

template<class Component>
Component& World::getComponent(Entity_ID entity)
{
  void* value = getComponent(entity, CPP_Type<Component>::id());
  return *reinterpret_cast<Component*>(value);
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