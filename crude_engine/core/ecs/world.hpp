#pragma once

#include <core/ecs/entity.hpp>
#include <queue>

namespace crude_engine
{

class World
{
public:
  World();

  Entity entity();
  Entity::ID newID();

private:
  std::queue<Entity::ID>  m_freeIDs;
  uint64                  m_newFreeID;
};

}