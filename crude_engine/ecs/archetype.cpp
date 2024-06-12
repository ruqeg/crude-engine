#include <vector>

module crude.ecs.archetype;

import crude.core.assert;
import crude.ecs.world;
import crude.ecs.component_register;

namespace crude::ecs
{

Archetype::Archetype(core::shared_ptr<Component_Register> componentRegister, Archetype_ID id, const core::set<Component_ID>& type)
  :
  m_componentRegister(componentRegister),
  m_id(id),
  m_type(type),
  m_componentsDataCapacity(0u),
  m_componentsDataSize(0u),
  m_entitiesNum(0u)
{
  m_components.resize(type.size());
  
  core::uint64 column = 0u;
  for (auto& component : type)
  {
    m_components[column].m_component = component;
    column++;
  }
}

Archetype::~Archetype()
{
  clear();
}

void Archetype::clear()
{
  for (Archetype_Column& column : m_components)
  {
    const Component_Register::Component_Info& componentInfo = m_componentRegister->getComponentInfo(column.m_component);

    for (core::uint32 row = 0; row < m_componentsDataSize; ++row)
    {
      componentInfo.fnDestroy(column.m_elements.data() + row * componentInfo.bsize);
    }

    column.m_elements.clear();
  }
  
  m_componentsDataCapacity = 0u;
  m_componentsDataSize = 0u;
  m_entitiesNum = 0u;
  m_freeRows = {};
}

void Archetype::increaseEntity(core::int64 num)
{
  m_entitiesNum += num;
}

void Archetype::reduceEntity(core::int64 num)
{
  m_entitiesNum -= num;
}

bool Archetype::entityEmpty() const
{
  return (m_entitiesNum == 0u);
}

core::uint64 Archetype::getEntityNum() const
{
  return m_entitiesNum;
}

core::uint64 Archetype::newRow()
{
  if (!m_freeRows.empty())
  {
    const core::uint64 row = m_freeRows.front();
    m_freeRows.pop();
    deinitializeRowData(row);
    initializeRowData(row);
    return row;
  }

  if (m_componentsDataCapacity == 0u)
  {
    const core::uint32 row = 0u;
    m_componentsDataCapacity = 1u;
    m_componentsDataSize = 1u;
    increaseCapacity();
    initializeRowData(row);
    return row;
  }
  
  if (m_componentsDataCapacity <= m_componentsDataSize)
  {
    const core::uint32 row = m_componentsDataSize++;
    increaseCapacity();
    initializeRowData(row);
    return row;
  }

  const core::uint32 row = m_componentsDataSize++;
  initializeRowData(row);
  return row;
}

void Archetype::removeComponentData(core::uint64 row)
{
  m_freeRows.push(row);
}

void Archetype::copyComponentData(core::uint64 column, core::uint64 row, const void* value)
{
  core::assert(row < m_componentsDataSize);

  const Component_ID component = m_components[column].m_component;
  const Component_Register::Component_Info& componentInfo = m_componentRegister->getComponentInfo(component);
  const core::uint64 index = row * componentInfo.bsize;
  void* pComponentData = m_components[column].m_elements.data() + index;

  componentInfo.fnCopy(pComponentData, value);
}

void Archetype::moveComponentData(core::uint64 column, core::uint64 row, void* value)
{
  core::assert(row < m_componentsDataSize);

  const Component_ID component = m_components[column].m_component;
  const Component_Register::Component_Info& componentInfo = m_componentRegister->getComponentInfo(component);
  const core::uint64 index = row * componentInfo.bsize;
  void* pComponentData = m_components[column].m_elements.data() + index;

  componentInfo.fnMove(pComponentData, value);
}

void* Archetype::getComponentData(core::uint64 column, core::uint64 row)
{
  core::assert(row < m_componentsDataSize);

  const Component_ID component = m_components[column].m_component;
  const Component_Register::Component_Info& componentInfo = m_componentRegister->getComponentInfo(component);
  const core::uint64 index = row * componentInfo.bsize;
  void* pComponentData = m_components[column].m_elements.data() + index;

  return reinterpret_cast<void*>(pComponentData);
}

const void* Archetype::getComponentData(core::uint64 column, core::uint64 row) const
{
  return reinterpret_cast<const void*>(getComponentData(column, row));
}

core::uint64 Archetype::getRowsNum() const
{
  return m_componentsDataSize;
}

const core::set<Component_ID>& Archetype::type() const
{
  return m_type;
}

Archetype_ID Archetype::id() const
{
  return m_id;
}

core::uint64 Archetype::getColumnsNum() const
{
  return m_components.size();
}

void Archetype::increaseCapacity()
{
  m_componentsDataCapacity *= 2u;

  for (auto& column : m_components)
  {
    const Component_Register::Component_Info& componentInfo = m_componentRegister->getComponentInfo(column.m_component);
    column.m_elements.resize(m_componentsDataCapacity * componentInfo.bsize);
  }
}

void Archetype::initializeRowData(core::uint64 row)
{
  for (auto& column : m_components)
  {
    const Component_Register::Component_Info& componentInfo = m_componentRegister->getComponentInfo(column.m_component);
    componentInfo.fnCreate(column.m_elements.data() + row * componentInfo.bsize);
  }
}

void Archetype::deinitializeRowData(core::uint64 row)
{
  for (auto& column : m_components)
  {
    const Component_Register::Component_Info& componentInfo = m_componentRegister->getComponentInfo(column.m_component);
    componentInfo.fnDestroy(column.m_elements.data() + row * componentInfo.bsize);
  }
}

}
