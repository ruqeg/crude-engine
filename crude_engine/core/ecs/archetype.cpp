#include <core/ecs/archetype.hpp>
#include <core/ecs/world.hpp>
#include <core/ecs/component_register.hpp>

namespace crude_engine
{

Archetype::Archetype(Component_Register* pComponentRegister, Archetype_ID id, const std::set<Component_ID>& type)
  :
  m_pComponentRegister(pComponentRegister),
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
  const Component_Register::Component_Info& componentInfo = m_pComponentRegister->getComponentInfo(component);
  const uint64 index = row * componentInfo.bsize;
  void* pComponentData = m_components[column].m_elements.data() + index;

  componentInfo.fnCopy(pComponentData, value);
}

void Archetype::moveComponentData(uint64 column, uint64 row, void* value)
{
  CRUDE_ASSERT(row < m_componentsDataSize);

  const Component_ID component = m_components[column].m_component;
  const Component_Register::Component_Info& componentInfo = m_pComponentRegister->getComponentInfo(component);
  const uint64 index = row * componentInfo.bsize;
  void* pComponentData = m_components[column].m_elements.data() + index;

  componentInfo.fnMove(pComponentData, value);
}

void* Archetype::getComponentData(uint64 column, uint64 row)
{
  CRUDE_ASSERT(row < m_componentsDataSize);

  const Component_ID component = m_components[column].m_component;
  const Component_Register::Component_Info& componentInfo = m_pComponentRegister->getComponentInfo(component);
  const uint64 index = row * componentInfo.bsize;
  void* pComponentData = m_components[column].m_elements.data() + index;

  return reinterpret_cast<void*>(pComponentData);
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
  m_componentsDataCapacity *= 2u;

  for (auto& column : m_components)
  {
    const Component_Register::Component_Info& componentInfo = m_pComponentRegister->getComponentInfo(column.m_component);
    column.m_elements.resize(m_componentsDataCapacity * componentInfo.bsize);
  }
}

}