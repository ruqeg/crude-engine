#pragma once

#include <core/std_containers.hpp>
#include <ecs/world.hpp>
#include <tuple>

namespace crude_engine
{

template<class ...Components>
class Query
{
private:
  static constexpr uint64 cFunctionComponentsNum = sizeof...(Components);

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
  explicit Query(World* world);
  explicit Query(World* world, const array<Component_ID, cFunctionComponentsNum>& components);

  template <class Func>
  void each(const Func& func);

private:
  template<typename Func, typename Tuple, int... S>
  void callFunction(Func&& func, Tuple&& params, Seq<S...>);

  template<typename Func, typename Tuple>
  void callFunction(Func&& func, Tuple&& params);

private:
  World*                                       m_world;
  array<Component_ID, cFunctionComponentsNum>  m_components;
};

}

#include <ecs/query.inl>