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

void World::add(Entity_ID entity, Component_ID component)
{
  bool noArchetypeForEntity = m_entityToRecord.find(entity) == m_entityToRecord.end();
  if (noArchetypeForEntity)
  {
    bool noArchetypeForSoloComponent = m_componentToArchetypeRecord.find(component) == m_componentToArchetypeRecord.end();
    if (noArchetypeForSoloComponent)
    {
      Archetype newArchertpe;

      newArchertpe.m_type  = { component };

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
      newArchertpe.m_components.resize(1u);
      newArchertpe.m_componentsNum = 1u;
      m_archetypes.push_back(newArchertpe);

      Archetype_Map newArchetypeMap;

      Archetype_Record newArchRec;
      newArchRec.column = 0u;

      newArchetypeMap[newArchertpe.m_id] = newArchRec;

      m_componentToArchetypeRecord[component] = newArchetypeMap;

      Record rec;
      rec.pArchetype = &m_archetypes.back();
      rec.row;
      m_entityToRecord[entity] = rec;
    }
  }
  else
  {
    Record record = m_entityToRecord[entity];
    bool noArchetypeForAddComponent = record.pArchetype->m_edges.find(component) == record.pArchetype->m_edges.end();
    if (noArchetypeForAddComponent)
    {
      Archetype newArchertpe;
      newArchertpe.m_type = record.pArchetype->m_type;
      newArchertpe.m_type.push_back(component);
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
      newArchertpe.m_components.resize(newArchertpe.m_type.size());
      newArchertpe.m_componentsNum = 1u;

      if (newArchertpe.m_id < m_archetypes.size())
      {
        m_archetypes[newArchertpe.m_id] = newArchertpe;
      }
      else
      {
        m_archetypes.push_back(newArchertpe);
      }

      Archetype_Map newArchetypeMap;

      Archetype_Record newArchRec;
      newArchRec.column = 0u;

      newArchetypeMap[newArchertpe.m_id] = newArchRec;

      for (auto& oldArchetypeComponent : record.pArchetype->m_type)
      {
        m_componentToArchetypeRecord[oldArchetypeComponent] = newArchetypeMap;
      }
      m_componentToArchetypeRecord[component] = newArchetypeMap;

      Record rec;
      rec.pArchetype = &m_archetypes[newArchertpe.m_id];
      rec.row;
      m_entityToRecord[entity] = rec;
    }

    record.pArchetype->m_componentsNum -= 1;
    if (record.pArchetype->m_componentsNum == 0)
    {
      m_freeArchetypeIDs.push(record.pArchetype->m_id);
    }
  }
  //Archetype& next_archetype = archetype.m_edges[component].m_add;
  //move_entity(archetype, record.m_row, next_archetype);
}

bool World::hasComponent(Entity_ID entity, Component_ID component) const
{
  const Record& record = m_entityToRecord.at(entity);
  const Archetype* pArchetype = record.pArchetype;
  const Archetype_Map& archetype_set = m_componentToArchetypeRecord.at(component);
  return archetype_set.count(pArchetype->m_id) != 0;
}

void World::remove(Entity_ID id)
{
  m_freeEntityIDs.push(id);
}

}