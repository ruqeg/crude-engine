#pragma once

#include <tuple>
#include <core/utility.hpp>
#include <functional>
#include <ecs/alias.hpp>
#include <array>
#include <algorithm>

namespace crude_engine
{

class World;

template<class ...Components>
class Query
{
private:
  template<int ...>
  struct Seq {};

  template<int N, int ...S>
  struct Gens : Gens<N - 1, N - 1, S...> { };

  template<int ...S>
  struct Gens<0, S...>
  {
    using Type = Seq<S...>;
  };

public:
  Query();
  explicit Query(World* world, std::vector<Component_ID> components);

  template <class Func>
  void each(const Func& func);

private:
  template<typename Func, typename Tuple, int... S>
  void callFunction(Func&& func, Tuple&& params, Seq<S...>);

  template<typename Func, typename Tuple>
  void callFunction(Func&& func, Tuple&& params);

private:
  World*                     m_world;
  std::vector<Component_ID>  m_components;
};

}

#include <ecs/world.hpp>
#include <ecs/query.inl>