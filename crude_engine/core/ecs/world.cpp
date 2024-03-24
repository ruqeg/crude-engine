#include <core/ecs/world.hpp>
#include <core/ecs/entity.hpp>

namespace crude_engine
{

World::World()
{
  m_archetypes.reserve(1000);
}

Entity World::entity()
{
  return Entity(this, newID());
}

Entity_ID World::newID()
{
  Entity_ID id = m_entityIDsManager.generate();
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
  archertpe.m_id = m_archetypeIDsManager.generate();

  addArchetypeToArray(archertpe);

  Archetype_Record archetypeRec;
  archetypeRec.column = 0u; // !TODO
  
  for (auto& archetypeComponent : type)
  {
    m_componentToArchetypeRecord[archetypeComponent][archertpe.m_id] = archetypeRec;
  }

  Record entityRec;
  entityRec.archetypeID = archertpe.m_id;
  entityRec.row; // !TODO
  m_entityToRecord[entity] = entityRec;
}

void World::assigneOrCreateArchetypeForEntity(Entity_ID entity, Component_ID component)
{
  Archetype_ID newArchetypeID;
  if (findArchetypeWithComponent(component, newArchetypeID))
  {
    Archetype& archetype = getArchetypeFromID(newArchetypeID);

    archetype.m_componentsNum++;

    Record entityRec;
    entityRec.archetypeID = newArchetypeID;
    entityRec.row = archetype.m_components.size() - 1;
    m_entityToRecord[entity] = entityRec;
  }
  else
  {
    createArchetypeForEntity(entity, { component });
  }
}

void World::reassigneArchetypeForEntity(Entity_ID entity, Component_ID component)
{
  Record record = m_entityToRecord[entity];
  Archetype& archetype = getArchetypeFromID(record.archetypeID);

  std::set<Component_ID> newEntityArchetypeType = archetype.m_type;
  newEntityArchetypeType.insert(component);

  archetype.m_componentsNum -= 1;
  if (archetype.m_componentsNum == 0)
  {
    for (auto& archetypeComponent : archetype.m_type)
    {
      m_componentToArchetypeRecord[archetypeComponent].erase(archetype.m_id);
    }

    m_archetypeIDsManager.destroy(archetype.m_id);
  }

  Archetype_ID newArchetypeID;
  if (findArchetypeWithComponent(component, newEntityArchetypeType, newArchetypeID))
  {
    Archetype& newArchetype = getArchetypeFromID(newArchetypeID);
    newArchetype.m_componentsNum++;

    Record rec;
    rec.row;
    rec.archetypeID = newArchetypeID;
    m_entityToRecord[entity] = rec;
  }
  else
  {
    createArchetypeForEntity(entity, newEntityArchetypeType);
  }
}

bool World::findArchetypeWithComponent(Component_ID component, const std::set<Component_ID>& type, Archetype_ID& archetypeID)
{
  if (m_componentToArchetypeRecord.find(component) != m_componentToArchetypeRecord.end())
  {
    Archetype_Map archetypeMap = m_componentToArchetypeRecord.at(component);
    for (const auto& archetypeRec : archetypeMap)
    {
      Archetype& archetype = getArchetypeFromID(archetypeRec.first);
      if (archetype.m_type == type)
      {
        archetypeID = archetypeRec.first;
        return true;
      }
    }
  }
  return false;
}

bool World::findArchetypeWithComponent(Component_ID component, Archetype_ID& archetypeID)
{
  if (m_componentToArchetypeRecord.find(component) != m_componentToArchetypeRecord.end())
  {
    const Archetype_Map& archetypeMap = m_componentToArchetypeRecord.at(component);
    for (auto& archetypeRec : archetypeMap)
    {
      Archetype& archetype = getArchetypeFromID(archetypeRec.first);
      if (archetype.m_type.size() == 1)
      {
        archetypeID = archetypeRec.first;
        return true;
      }
    }
  }
  return false;
}

void World::addArchetypeToArray(const Archetype& archetype)
{
  // !UNSAFE
  ID_Index index = ID_Manager::getIndex(archetype.m_id);
  if (index < m_archetypes.size())
  {
    m_archetypes[index] = archetype;
  }
  else
  {
    m_archetypes.push_back(archetype);
  }
}

Archetype& World::getArchetypeFromID(const Archetype_ID archetypeID)
{
  return m_archetypes[ID_Manager::getIndex(archetypeID)];
}

bool World::hasComponent(Entity_ID entity, Component_ID component) const
{
  // entity not created or archetype with the component doen't exist 
  if ((m_entityToRecord.find(entity) == m_entityToRecord.end()) || (m_componentToArchetypeRecord.find(component) == m_componentToArchetypeRecord.end()))
  {
    return false;
  }

  const Record& record = m_entityToRecord.at(entity);
  const Archetype_Map& archetypeSet = m_componentToArchetypeRecord.at(component);
  return (archetypeSet.count(record.archetypeID) != 0);
}

void World::remove(Entity_ID id)
{
  m_entityIDsManager.destroy(id);
}

}