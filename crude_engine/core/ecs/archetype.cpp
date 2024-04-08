#include <core/ecs/archetype.hpp>
#include <core/ecs/world.hpp>

namespace crude_engine
{

Archetype::Archetype(World* world, Archetype_ID id, const std::set<Component_ID>& type)
  :
  m_world(world),
  m_id(id),
  m_type(type),
  m_componentsDataCapacity(0u),
  m_componentsDataSize(0u),
  m_entitiesNum(0u)
{
  m_components.resize(type.size());
  
  uint64 column = 0u;
  for (auto& component : type)
  {
    m_components[column].m_component = component;
    column++;
  }
}

void Archetype::increaseEntity(int64 num)
{
  m_entitiesNum += num;
}

void Archetype::reduceEntity(int64 num)
{
  m_entitiesNum -= num;
}

bool Archetype::entityEmpty() const
{
  return (m_entitiesNum == 0u);
}

uint64 Archetype::getEntityNum() const
{
  return m_entitiesNum;
}

uint64 Archetype::newRow()
{
  if (!m_freeRows.empty())
  {
    const uint64 row = m_freeRows.front();
    m_freeRows.pop();
    return row;
  }

  if (m_componentsDataCapacity == 0u)
  {
    const uint32 row = 0u;
    m_componentsDataCapacity = 1u;
    m_componentsDataSize = 1u;
    increaseCapacity();

    return row;
  }
  
  if (m_componentsDataSize < m_componentsDataCapacity)
  {
    const uint32 row = m_componentsDataSize++;
    return row;
  }

  const uint32 row = m_componentsDataSize++;
  increaseCapacity();
  return row;
}

void Archetype::removeComponentData(uint64 row)
{
  m_freeRows.push(row);
}

void Archetype::copyComponentData(uint64 column, uint64 row, const void* value)
{
  CRUDE_ASSERT(row < m_componentsDataSize);
  const Component_ID component = m_components[column].m_component;
  const uint64 elementSize = m_world->m_componentToElementSize.at(component);
  Copy_Component_Functinon funCopy = m_world->m_componentToCopyFunc.at(component);
  funCopy(m_components[column].m_elements.data() + (row * elementSize), value);
}

void Archetype::moveComponentData(uint64 column, uint64 row, void* value)
{
  CRUDE_ASSERT(row < m_componentsDataSize);
  const Component_ID component = m_components[column].m_component;
  const uint64 elementSize = m_world->m_componentToElementSize.at(component);
  Move_Component_Functinon funMove = m_world->m_componentToMoveFunc.at(component);
  funMove(m_components[column].m_elements.data() + (row * elementSize), value);
}

void* Archetype::getComponentData(uint64 column, uint64 row)
{
  CRUDE_ASSERT(m_world->m_componentToElementSize.find(m_components[column].m_component) != m_world->m_componentToElementSize.end());
  CRUDE_ASSERT(row < m_componentsDataSize);

  const uint64 elementSize = m_world->m_componentToElementSize.at(m_components[column].m_component);
  return reinterpret_cast<void*>(m_components[column].m_elements.data() + (row * elementSize));
}

const void* Archetype::getComponentData(uint64 column, uint64 row) const
{
  return reinterpret_cast<const void*>(getComponentData(column, row));
}

const std::set<Component_ID>& Archetype::getType() const
{
  return m_type;
}

Archetype_ID Archetype::getID() const
{
  return m_id;
}

uint64 Archetype::getComponentsNum() const
{
  return m_components.size();
}

void Archetype::increaseCapacity()
{
  m_componentsDataCapacity *= 2;

  for (auto& column : m_components)
  {
    const uint64 elementSize = m_world->m_componentToElementSize.at(column.m_component);
    column.m_elements.resize(m_componentsDataCapacity * elementSize);
  }
}

}
