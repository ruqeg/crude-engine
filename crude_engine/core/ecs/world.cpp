#include <core/ecs/world.hpp>
#include <core/ecs/entity.hpp>

namespace crude_engine
{

World::World()
{
  m_newFreeEntityID = 0u;
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
    m_freeEntityIDs.front();
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
      newArchertpe.m_id = m_newFreeArchetypeID++;
      newArchertpe.m_edges.clear();
      newArchertpe.m_components.resize(1u);

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

  }
  //Archetype& next_archetype = archetype.m_edges[component].m_add;
  //move_entity(archetype, record.m_row, next_archetype);
}

bool World::hasComponent(Entity_ID entity, Component_ID component)
{
  const Record& record = m_entityToRecord[entity];
  const Archetype* pArchetype = record.pArchetype;
  const Archetype_Map& archetype_set = m_componentToArchetypeRecord[component];
  return archetype_set.count(pArchetype->m_id) != 0;
}

void World::remove(Entity_ID id)
{
  m_freeEntityIDs.push(id);
}

}