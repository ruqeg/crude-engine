#pragma once

#include <ecs/alias.hpp>
#include <functional>

namespace crude_engine
{

class World;

template<class... Components>
class Query
{
public:
  Query(World* world, std::vector<Component_ID> components);

  template <class Func>
  void each(const Func& func) const;

private:
  World*                     m_world;
  std::vector<Component_ID>  m_components;
};

}