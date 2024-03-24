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
  // no archetype for entity
  if (m_entityToRecord.find(entity) == m_entityToRecord.end())
  {
    // no archetype for component
    if (m_componentToArchetypeRecord.find(component) == m_componentToArchetypeRecord.end())
    {
      createArchetypeForEntity(entity, { component });
    }
    else
    {
      assigneOrCreateArchetypeForEntity(entity, component);
    }
  }
  else
  {
    reassigneArchetypeForEntity(entity, component);
  }
}

void World::createArchetypeForEntity(Entity_ID entity, const std::set<Component_ID>& type)
{
  Archetype archertpe;
  archertpe.m_type = type;
  archertpe.m_edges.clear(); // !TODO
  archertpe.m_components.resize(type.size());
  archertpe.m_componentsNum = 1u;
  
  if (m_freeArchetypeIDs.empty())
  {
    archertpe.m_id = m_newFreeArchetypeID++;
  }
  else
  {
    archertpe.m_id = m_freeArchetypeIDs.front();
    m_freeArchetypeIDs.pop();
  }

  if (archertpe.m_id < m_archetypes.size())
  {
    m_archetypes[archertpe.m_id] = archertpe;
  }
  else
  {
    m_archetypes.push_back(archertpe);
  }

  Archetype_Record archetypeRec;
  archetypeRec.column = 0u; // !TODO
  
  for (auto& archetypeComponent : type)
  {
    m_componentToArchetypeRecord[archetypeComponent][archertpe.m_id] = archetypeRec;
  }

  Record entityRec;
  entityRec.pArchetype = &m_archetypes[archertpe.m_id];
  entityRec.row; // !TODO
  m_entityToRecord[entity] = entityRec;
}

void World::assigneOrCreateArchetypeForEntity(Entity_ID entity, Component_ID component)
{
  Archetype* pArchetype = nullptr;
  const Archetype_Map& archetypeMap = m_componentToArchetypeRecord.at(component);
  for (auto& archetypeRec : archetypeMap)
  {
    Archetype_ID index = archetypeRec.first;
    Archetype* pArchetypeCandidate = &m_archetypes[index];
    if (pArchetypeCandidate->m_type.size() == 1)
    {
      pArchetype = pArchetypeCandidate;
    }
  }

  if (pArchetype == nullptr)
  {
    createArchetypeForEntity(entity, { component });
  }
  else
  {
    Record entityRec;
    entityRec.pArchetype = pArchetype;
    entityRec.row = pArchetype->m_components.size() - 1;
    entityRec.pArchetype->m_componentsNum++;
    m_entityToRecord[entity] = entityRec;
  }
}

void World::reassigneArchetypeForEntity(Entity_ID entity, Component_ID component)
{
  Record record = m_entityToRecord[entity];

  std::set<Component_ID> newEntityArchetypeType = record.pArchetype->m_type;
  newEntityArchetypeType.insert(component);

  record.pArchetype->m_componentsNum -= 1;
  if (record.pArchetype->m_componentsNum == 0)
  {
    for (auto& archetypeComponent : record.pArchetype->m_type)
    {
      m_componentToArchetypeRecord[archetypeComponent].erase(record.pArchetype->m_id);
    }

    m_freeArchetypeIDs.push(record.pArchetype->m_id);
  }

  Archetype_ID newArchetype;
  if (findArchetypeWithComponent(component, newEntityArchetypeType, newArchetype))
  {
    Record rec;
    rec.row;
    rec.pArchetype = &m_archetypes[newArchetype];
    rec.pArchetype->m_componentsNum++;
    m_entityToRecord[entity] = rec;
  }
  else
  {
    createArchetypeForEntity(entity, newEntityArchetypeType);
  }
}

bool World::findArchetypeWithComponent(Component_ID component, const std::set<Component_ID>& type, Archetype_ID& dstAcrhetypeID)
{
  if (m_componentToArchetypeRecord.find(component) != m_componentToArchetypeRecord.end())
  {
    Archetype_Map archetypeToRec = m_componentToArchetypeRecord.at(component);
    for (auto& archetypeRec : archetypeToRec)
    {
      if (m_archetypes[archetypeRec.first].m_type == type)
      {
        dstAcrhetypeID = archetypeRec.first;
        return true;
      }
    }
  }
  return false;
}

bool World::hasComponent(Entity_ID entity, Component_ID component) const
{
  // entity not created or archetype with the component doen't exist 
  if ((m_entityToRecord.find(entity) == m_entityToRecord.end()) || (m_componentToArchetypeRecord.find(component) == m_componentToArchetypeRecord.end()))
  {
    return false;
  }

  const Record& record = m_entityToRecord.at(entity);
  const Archetype* pArchetype = record.pArchetype;
  const Archetype_Map& archetypeSet = m_componentToArchetypeRecord.at(component);
  return (archetypeSet.count(pArchetype->m_id) != 0);
}

void World::remove(Entity_ID id)
{
  m_freeEntityIDs.push(id);
}

}