module;

#include <utility>
#include <tuple>
#include <iterator>

export module crude.ecs.query;

import crude.core.std_containers_stack;
import crude.core.std_containers_heap;
import crude.ecs.alias;
import crude.ecs.world;

export namespace crude::ecs
{

template<class ...Components>
class Query
{
private:
  static constexpr core::uint64 cFunctionComponentsNum = sizeof...(Components);

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
  explicit Query(World* world, const core::array<Component_ID, cFunctionComponentsNum>& components);

  template <class Func>
  void each(const Func& func);

private:
  template<typename Func, typename Tuple, int... S>
  void callFunction(Func&& func, Tuple&& params, Seq<S...>);

  template<typename Func, typename Tuple>
  void callFunction(Func&& func, Tuple&& params);

private:
  World*                                             m_world;
  core::array<Component_ID, cFunctionComponentsNum>  m_components;
};

template<class ...Components>
Query<Components...>::Query()
  :
  m_world(nullptr)
{}

template<class ...Components>
Query<Components...>::Query(World* world)
{
  *this = world->query<Components...>();
}

template<class ...Components>
Query<Components...>::Query(World* world, const core::array<Component_ID, cFunctionComponentsNum>& components)
{
  m_world = world;
  m_components = components;
}

template<class ...Components>
template<class Func>
void Query<Components...>::each(const Func& func)
{
  //!TODO currently its sooo slow

  for (Archetype& archetype : m_world->m_archetypes)
  {
    core::array<core::uint64, cFunctionComponentsNum> funcIdxToArchetypeIdx;

    core::uint64 matchComponentsNum = 0;
    for (core::uint64 funcComponentIndex = 0u; funcComponentIndex < cFunctionComponentsNum; ++funcComponentIndex)
    {
      const auto& archetypeType = archetype.type();

      const auto archetypeComponentIt = archetypeType.find(m_components[funcComponentIndex]);
      if (archetypeComponentIt == archetypeType.end())
        continue;

      const core::uint64 archetypeComponentIndex = std::distance(archetypeType.begin(), archetypeComponentIt);
      funcIdxToArchetypeIdx[funcComponentIndex] = archetypeComponentIndex;
      matchComponentsNum++;
    }

    if (matchComponentsNum == cFunctionComponentsNum)
    {
      for (core::uint64 row = 0u; row < archetype.getRowsNum(); ++row)
      {
        std::tuple<Components*...> funcArgumnetsPtr;

        auto fnSetFuncArgument = []<class Component>(Component * *funcArgumentPtr, void* componentRowData) {
          *funcArgumentPtr = reinterpret_cast<Component*>(componentRowData);
        };

        auto fsSetFuncArgumentToComponentRowData = [&]<auto... I>(std::index_sequence<I...>) {
          (fnSetFuncArgument(&std::get<I>(funcArgumnetsPtr), archetype.getComponentData(funcIdxToArchetypeIdx[I], row)), ...);
        };
        fsSetFuncArgumentToComponentRowData(std::make_index_sequence<cFunctionComponentsNum>{});

        callFunction(func, std::forward<std::tuple<Components*...>>(funcArgumnetsPtr));
      }
    }
  }
}

template<class ...Components>
template<typename Func, typename Tuple>
void Query<Components...>::callFunction(Func&& func, Tuple&& params)
{
  callFunction(std::forward<Func>(func), std::forward<Tuple>(params), typename Gens<std::tuple_size<Tuple>::value>::Type());
}

template<class ...Components>
template<typename Func, typename Tuple, int ...S>
void Query<Components...>::callFunction(Func&& func, Tuple&& params, Seq<S...>)
{
  func((*std::get<S>(std::forward<Tuple>(params))) ...);
}

}