#include <core/ecs/world.hpp>

namespace crude_engine
{

template<class Component>
void World::addComponent(Entity_ID entity)
{
  m_componentRegister.registerComponent<Component>();
  addComponent(entity, CPP_Type<Component>::id());
}

template<class Component>
void World::setComponent(Entity_ID entity, const Component& value)
{
  m_componentRegister.registerComponent<Component>();
  setComponent(entity, CPP_Type<Component>::id(), &value);
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
  void* value = getComponent(entity, CPP_Type<Component>::id());
  return *reinterpret_cast<Component*>(value);
}

}