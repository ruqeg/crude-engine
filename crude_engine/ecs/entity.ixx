module;

export module crude.ecs.entity;

import crude.ecs.alias;
import crude.ecs.world;

export namespace crude::ecs
{

class Entity
{
public:
  Entity();
  explicit Entity(World* world);
  explicit Entity(const World* world, Entity_ID id);

  Entity_ID id() const;

  template <class Component>
  Entity& add();

  template <class Component>
  Entity& set(const Component& value);

  template <class Component>
  const Component& get() const;

  template <class Component>
  Component& get();

  //template <class Component>
  //Entity& set(Component&& component);

  template <class Component>
  bool hasComponent() const;

  template <class Component>
  const Entity& remove() const;

  void clear();
  void destruct();

private:
  Entity_ID  m_id;
  World*     m_world;

  friend class World;
};

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


} //namespace crude