#pragma once

#include <core/alias.hpp>

namespace crude_engine
{

class World;

class Entity
{
public:
  using ID = uint64;

public:
  Entity();
  explicit Entity(World* world);
  explicit Entity(const World* world, ID id);

  ID id() const;

  bool isValid() const;
  bool isAlive() const;
  explicit operator bool() const;

  template <class Component>
  const Component& get() const;

  template <class Component>
  void add() const;

  template <class Component>
  void remove() const;

  void clear();
  void destruct();

private:
  ID      m_id;
  World*  m_world;

  friend class World;
};

} //namespace crude_engine