#pragma once

#include <core/ecs/world.hpp>

namespace crude_engine
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
  void remove() const;

  void clear();
  void destruct();

private:
  Entity_ID  m_id;
  World*     m_world;

  friend class World;
};

} //namespace crude_engine

#include <core/ecs/entity.inl>