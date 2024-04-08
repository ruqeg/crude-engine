#include <core/ecs/archetype.hpp>
#include <core/ecs/world.hpp>

// !TODO do something with this code pls

namespace crude_engine
{

Archetype::Archetype(World* world, Archetype_ID id, const std::set<Component_ID>& type)
  :
  m_world(world),
  m_id(id),
  m_type(type),
  m_entitesCapacity(0u),
  m_entitesSize(0u),
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

void Archetype::addEntity()
{
  m_entitiesNum++;
}

void Archetype::removeEntity()
{
  m_entitiesNum--;
}

bool Archetype::empty()
{
  return (m_entitiesNum == 0u);
}

uint64 Archetype::newRow()
{
  for (auto& component : m_type)
  {
    CRUDE_ASSERT(m_world->m_componentToElementSize.find(component) != m_world->m_componentToElementSize.end());
    CRUDE_ASSERT(m_world->m_componentToCopyFunc.find(component) != m_world->m_componentToCopyFunc.end());
    CRUDE_ASSERT(m_world->m_componentToMoveFunc.find(component) != m_world->m_componentToMoveFunc.end());
  }

  uint64 row;

  // !TODO make this code more readable ||_||
  if (!m_freeRows.empty())
  {
    row = m_freeRows.front();
    m_freeRows.pop();
  }
  else if (m_entitesCapacity == 0u)
  {
    row = 0u;
    m_entitesCapacity = 4u;
    m_entitesSize = 1u;

    for (auto& column : m_components)
    {
      const uint64 elementSize = m_world->m_componentToElementSize.at(column.m_component);
      column.m_elements.resize(m_entitesCapacity * elementSize);
    }
  }
  else if (m_entitesSize < m_entitesCapacity)
  {
    row = m_entitesSize;
    m_entitesSize++;
  }
  else
  {
    // !TODO it should be better ||_||
    row = m_entitesSize;
    m_entitesCapacity += 10;
    for (auto& column : m_components)
    {
      const uint64 elementSize = m_world->m_componentToElementSize.at(column.m_component);
      column.m_elements.resize(m_entitesCapacity * elementSize);
    }
    m_entitesSize++;
  }

  return row;
}

void Archetype::remove(uint64 row)
{
  m_freeRows.push(row);
}

// !TODO dont dublicate code
void Archetype::set(uint64 column, uint64 row, const void* value)
{
  for (auto& component : m_type)
  {
    CRUDE_ASSERT(m_world->m_componentToElementSize.find(component) != m_world->m_componentToElementSize.end());
    CRUDE_ASSERT(m_world->m_componentToCopyFunc.find(component) != m_world->m_componentToCopyFunc.end());
    CRUDE_ASSERT(m_world->m_componentToMoveFunc.find(component) != m_world->m_componentToMoveFunc.end());
  }
  CRUDE_ASSERT(row < m_entitesSize);

  const Component_ID component = m_components[column].m_component;

  const uint64 elementSize = m_world->m_componentToElementSize.at(component);
  Copy_Component_Functinon funCopy = m_world->m_componentToCopyFunc.at(component);
  funCopy(m_components[column].m_elements.data() + (row * elementSize), value);
}

void* Archetype::get(uint64 column, uint64 row)
{
  CRUDE_ASSERT(m_world->m_componentToElementSize.find(m_components[column].m_component) != m_world->m_componentToElementSize.end());
  CRUDE_ASSERT(row < m_entitesSize);

  const uint64 elementSize = m_world->m_componentToElementSize.at(m_components[column].m_component);
  return reinterpret_cast<void*>(m_components[column].m_elements.data() + (row * elementSize));
}

const void* Archetype::get(uint64 column, uint64 row) const
{
  CRUDE_ASSERT(m_world->m_componentToElementSize.find(m_components[column].m_component) != m_world->m_componentToElementSize.end());
  CRUDE_ASSERT(row < m_entitesSize);

  const uint64 elementSize = m_world->m_componentToElementSize.at(m_components[column].m_component);
  return reinterpret_cast<const void*>(m_components[column].m_elements.data() + (row * elementSize));
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

}
