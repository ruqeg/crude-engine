#include <core/ecs/world.hpp>

namespace crude_engine
{

World::World()
{
  m_newFreeID = 0u;
}

Entity World::entity()
{
  return Entity(this, newID());
}

Entity::ID World::newID()
{
  Entity::ID id;

  if (!m_freeIDs.empty())
  {
    id = m_freeIDs.front();
    m_freeIDs.pop();
  }
  else
  {
    id = m_newFreeID;
    ++m_newFreeID;
  }

  return id;
}

void World::remove(Entity::ID id)
{
  m_freeIDs.push(id);
}

}