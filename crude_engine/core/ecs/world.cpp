#include <core/ecs/world.hpp>
#include <core/ecs/entity.hpp>

namespace crude_engine
{

World::World()
{
  m_newFreeEntityID = 0u;
  m_archetypes.reserve(1000);
}

Entity World::entity()
{
  return Entity(this, newID());
}

Entity_ID World::newID()
{
  Entity_ID id;

  if (!m_freeEntityIDs.empty())
  {
    id = m_freeEntityIDs.front();
    m_freeEntityIDs.pop();
  }
  else
  {
    id = m_newFreeEntityID;
    ++m_newFreeEntityID;
  }

  return id;
}

void World::addComponent(Entity_ID entity, Component_ID component)
{
  bool noArchetypeForEntity = m_entityToRecord.find(entity) == m_entityToRecord.end();
  if (noArchetypeForEntity)
  {
    bool noArchetypeForSoloComponent = m_componentToArchetypeRecord.find(component) == m_componentToArchetypeRecord.end();
    if (noArchetypeForSoloComponent)
    {
      createArchetypeForComponent(entity, { component });
    }
    else
    {
      addComponentToArchetype(entity, component);
    }
  }
  else
  {
    addComponentToEntityArchetype(entity, component);
  }
}

void World::createArchetypeForComponent(Entity_ID entity, const std::set<Component_ID> type)
{
  Archetype newArchertpe;
  newArchertpe.m_type = type;

  if (m_freeArchetypeIDs.empty())
  {
    newArchertpe.m_id = m_newFreeArchetypeID++;
  }
  else
  {
    newArchertpe.m_id = m_freeArchetypeIDs.front();
    m_freeArchetypeIDs.pop();
  }
  newArchertpe.m_edges.clear();
  newArchertpe.m_components.resize(type.size());
  newArchertpe.m_componentsNum = 1u;

  if (newArchertpe.m_id < m_archetypes.size())
  {
    m_archetypes[newArchertpe.m_id] = newArchertpe;
  }
  else
  {
    m_archetypes.push_back(newArchertpe);
  }

  Archetype_Record newArchRec;
  newArchRec.column = 0u;

  for (auto& archetypeComponent : m_archetypes[newArchertpe.m_id].m_type)
  {
    m_componentToArchetypeRecord[archetypeComponent][newArchertpe.m_id] = newArchRec;
  }

  Record rec;
  rec.pArchetype = &m_archetypes[newArchertpe.m_id];
  rec.row;
  m_entityToRecord[entity] = rec;
}

void World::addComponentToArchetype(Entity_ID entity, Component_ID component)
{
  Archetype* pArchetype = nullptr;
  if (m_componentToArchetypeRecord.find(component) != m_componentToArchetypeRecord.end())
  {
    Archetype_Map archMap = m_componentToArchetypeRecord.at(component);
    for (auto& rec : archMap)
    {
      if (m_archetypes[rec.first].m_type.size() == 1)
      {
        pArchetype = &m_archetypes[rec.first];
      }
    }
  }

  if (pArchetype == nullptr)
  {
    createArchetypeForComponent(entity, { component });
  }
  else
  {
    Record rec;
    rec.pArchetype = pArchetype;
    rec.row = pArchetype->m_components.size() - 1;
    rec.pArchetype->m_componentsNum++;
    m_entityToRecord[entity] = rec;
  }
}

void World::addComponentToEntityArchetype(Entity_ID entity, Component_ID component)
{
  Record record = m_entityToRecord[entity];
  std::set<Component_ID> newType = record.pArchetype->m_type;
  newType.insert(component);

  Archetype* pArchetype = nullptr;
  if (m_componentToArchetypeRecord.find(component) != m_componentToArchetypeRecord.end())
  {
    Archetype_Map archMap = m_componentToArchetypeRecord.at(component);
    for (auto& rec : archMap)
    {
      if (m_archetypes[rec.first].m_type == newType)
      {
        pArchetype = &m_archetypes[rec.first];
      }
    }
  }

  record.pArchetype->m_componentsNum -= 1;
  if (record.pArchetype->m_componentsNum == 0)
  {
    for (auto& archetypeComponent : record.pArchetype->m_type)
    {
      m_componentToArchetypeRecord[archetypeComponent].erase(record.pArchetype->m_id);
    }

    m_freeArchetypeIDs.push(record.pArchetype->m_id);
  }

  if (pArchetype == nullptr)
  {
    createArchetypeForComponent(entity, newType);
  }
  else
  {
    Record rec;
    rec.row;
    rec.pArchetype = pArchetype;
    rec.pArchetype->m_componentsNum++;
    m_entityToRecord[entity] = rec;
  }
}

bool World::hasComponent(Entity_ID entity, Component_ID component) const
{
  if (m_entityToRecord.find(entity) == m_entityToRecord.end())
  {
    return false;
  }

  const Record& record = m_entityToRecord.at(entity);
  const Archetype* pArchetype = record.pArchetype;

  if (m_componentToArchetypeRecord.find(component) == m_componentToArchetypeRecord.end())
  {
    return false;
  }

  const Archetype_Map& archetype_set = m_componentToArchetypeRecord.at(component);
  return archetype_set.count(pArchetype->m_id) != 0;
}

void World::remove(Entity_ID id)
{
  m_freeEntityIDs.push(id);
}

}