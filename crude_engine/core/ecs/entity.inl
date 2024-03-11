#include <core/ecs/entity.hpp>

namespace crude_engine
{

template <class Component>
Entity& Entity::add()
{
  m_world->add(m_id, CPP_Type<Component>::id());
  return *this;
}


template <class Component>
bool Entity::hasComponent() const
{
  return m_world->hasComponent(m_id, CPP_Type<Component>::id());
}

}