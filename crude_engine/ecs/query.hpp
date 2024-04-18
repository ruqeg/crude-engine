#pragma once

#include <tuple>
#include <core/utility.hpp>
#include <ecs/world.hpp>
#include <functional>
#include <algorithm>

namespace crude_engine
{

class World;

template<int ...>
struct Seq {};

template<int N, int ...S>
struct Gens : Gens<N - 1, N - 1, S...> { };

template<int ...S>
struct Gens<0, S...> 
{
  using Type = Seq<S...>;
};

template<class ...Components>
class Query
{
public:
  Query(World* world);
  Query(World* world, std::vector<Component_ID> components);

  template <class Func>
  void each(const Func& func);

private:
  template<typename Func, typename Tuple, int... S>
  void iterate(Func&& func, Tuple&& params, Seq<S...>);

  template<typename Func, typename Tuple>
  void iterate(Func&& func, Tuple&& params);

private:
  World*                     m_world;
  std::vector<Component_ID>  m_components;
};

template<class ...Components>
Query<Components...>::Query(World* world)
{
  m_world = world;
  (m_components.push_back(CPP_Type<Components>::id()), ...);
}

template<class ...Components>
Query<Components...>::Query(World* world, std::vector<Component_ID> components)
{
  m_world = world;
  (m_components.push_back(CPP_Type<Components>::id()), ...);
}

template<class T>
void setComponent(T* e, Archetype& archetype, size_t column)
{
  for (int row = 0; row < archetype.getRowsNum(); ++row)
  { 
    e = reinterpret_cast<T*>(archetype.getComponentData(column, row));
  }
}

template<class Component>
void printElem(Component** dst, void* src) {
  *dst = reinterpret_cast<Component*>(src);
}

template<class ...Components>
template<class Func>
void Query<Components...>::each(const Func& func)
{
  for (Archetype& archetype : m_world->m_archetypes)
  {
    int same = 0;
    for (auto component : m_components)
    {
      for (auto archetypeComp : archetype.type())
      {
        if (component == archetypeComp)
          same++;
      }
    }

    if (same == m_components.size())
    {
      for (uint64 row = 0u; row < archetype.getRowsNum(); ++row)
      {
        std::tuple<Components*...> tpl;
        [&row, &tpl, &archetype] <auto... I>(std::index_sequence<I...>) {
          (printElem(&std::get<I>(tpl), archetype.getComponentData(I, row)), ...);
        }(std::make_index_sequence<std::tuple_size_v<decltype(tpl)>>{});
        iterate(std::forward<Func>(func), std::forward<std::tuple<Components*...>>(tpl));
      }
    }
  }
}

template<class ...Components>
template<typename Func, typename Tuple>
inline void Query<Components...>::iterate(Func&& func, Tuple&& params)
{
  iterate(Utility::forward<Func>(func), Utility::forward<Tuple>(params), typename Gens<std::tuple_size<Tuple>::value>::Type());
}

template<class ...Components>
template<typename Func, typename Tuple, int ...S>
inline void Query<Components...>::iterate(Func&& func, Tuple&& params, Seq<S...>)
{
  func((*std::get<S>(Utility::forward<Tuple>(params))) ...);
}

}