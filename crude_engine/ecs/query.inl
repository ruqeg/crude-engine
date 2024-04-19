#include <ecs/query.hpp>

namespace crude_engine
{
template<class ...Components>
Query<Components...>::Query()
  :
  m_world(nullptr)
{}

template<class ...Components>
Query<Components...>::Query(World* world, std::vector<Component_ID> components)
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
    constexpr uint64 funcComponentsNum = sizeof...(Components);
    std::array<uint64, funcComponentsNum> funcIdxToArchetypeIdx;

    uint64 matchComponentsNum = 0;
    for (uint64 funcComponentIndex = 0u; funcComponentIndex < funcComponentsNum; ++funcComponentIndex)
    {
      const auto& archetypeType = archetype.type();

      const auto archetypeComponentIt = archetypeType.find(m_components[funcComponentIndex]);
      if (archetypeComponentIt == archetypeType.end())
        continue;

      const uint64 archetypeComponentIndex = std::distance(archetypeType.begin(), archetypeComponentIt);
      funcIdxToArchetypeIdx[funcComponentIndex] = archetypeComponentIndex;
      matchComponentsNum++;
    }

    if (matchComponentsNum == funcComponentsNum)
    {
      for (uint64 row = 0u; row < archetype.getRowsNum(); ++row)
      {
        std::tuple<Components*...> funcArgumnetsPtr;

        auto fnSetFuncArgument = []<class Component>(Component * *funcArgumentPtr, void* componentRowData) {
          *funcArgumentPtr = reinterpret_cast<Component*>(componentRowData);
        };

        auto fsSetFuncArgumentToComponentRowData = [&]<auto... I>(std::index_sequence<I...>) {
          (fnSetFuncArgument(&std::get<I>(funcArgumnetsPtr), archetype.getComponentData(funcIdxToArchetypeIdx[I], row)), ...);
        };
        fsSetFuncArgumentToComponentRowData(std::make_index_sequence<funcComponentsNum>{});

        callFunction(func, std::forward<std::tuple<Components*...>>(funcArgumnetsPtr));
      }
    }
  }
}

template<class ...Components>
template<typename Func, typename Tuple>
void Query<Components...>::callFunction(Func&& func, Tuple&& params)
{
  callFunction(Utility::forward<Func>(func), Utility::forward<Tuple>(params), typename Gens<std::tuple_size<Tuple>::value>::Type());
}

template<class ...Components>
template<typename Func, typename Tuple, int ...S>
void Query<Components...>::callFunction(Func&& func, Tuple&& params, Seq<S...>)
{
  func((*std::get<S>(Utility::forward<Tuple>(params))) ...);
}

}