#include <ecs/entity.hpp>

namespace crude_engine
{

template <class Component>
Entity& Entity::add()
{
  m_world->addComponent<Component>(m_id);
  return *this;
}

template<class Component>
Entity& Entity::set(const Component& value)
{
  m_world->setComponent<Component>(m_id, value);
  return *this;
}

template<class Component>
const Component& Entity::get() const
{
  return m_world->getComponent<Component>(m_id);
}

template<class Component>
Component& Entity::get()
{
  return m_world->getComponent<Component>(m_id);
}

template <class Component>
bool Entity::hasComponent() const
{
  return m_world->hasComponent<Component>(m_id);
}

template<class Component>
const Entity& Entity::remove() const
{
  m_world->removeComponent<Component>(m_id);
  return *this;
}

}