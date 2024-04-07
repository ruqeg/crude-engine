#include <core/ecs/world.hpp>
#include <core/ecs/entity.hpp>
#include <core/algorithms.hpp>

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

void World::setComponent(Entity_ID entity, Component_ID component, const void* value)
{
  if (!hasComponent(entity, component))
  {
    // !TODO
    CRUDE_ASSERT(false && "TODO");
    return;
  }

  Record& entityRec = m_entityToRecord[entity];
  Archetype_Map& componentRec = m_componentToArchetypeRecord[component];
  Archetype& archetype = getArchetypeFromID(entityRec.archetypeID);
  Column& mcolumn = archetype.m_components[componentRec[entityRec.archetypeID].column];
  if (!entityRec.row.has_value())
  {
    for (auto& column : archetype.m_components)
    {
      size_t c = m_componentToElementSize[column.component];
      column.elements.resize(column.elements.size() + m_componentToElementSize[column.component]);
    }

    entityRec.row = static_cast<int64>(mcolumn.elements.size() / m_componentToElementSize[component]) - 1;
  }
  m_componentToCopyFunc[component](
    mcolumn.elements.data() + (entityRec.row.value() * m_componentToElementSize[component]), 
    value);
}

void* World::getComponent(Entity_ID entity, Component_ID component)
{
  if (!hasComponent(entity, component))
  {
    // !TODO
    CRUDE_ASSERT(false && "TODO");
  }

  Record& entityRec = m_entityToRecord[entity];
  Archetype_Map& componentRec = m_componentToArchetypeRecord[component];
  Archetype& archetype = getArchetypeFromID(entityRec.archetypeID);
  Column& mcolumn = archetype.m_components[componentRec[entityRec.archetypeID].column];
  void* value = mcolumn.elements.data() + (entityRec.row.value() * m_componentToElementSize[component]);
  return value;
}

void World::removeComponent(Entity_ID entity, Component_ID component)
{
  // no archetype for entity
  if (m_entityToRecord.find(entity) == m_entityToRecord.end())
  {
    return;
  }

  Record& entityRec = m_entityToRecord[entity];
  Archetype& archetype = getArchetypeFromID(entityRec.archetypeID);
  Archetype_Record& archetypeRec = m_componentToArchetypeRecord[component][entityRec.archetypeID];

  std::set<Component_ID> newEntityArchetypeType = archetype.m_type;
  newEntityArchetypeType.erase(component);

  archetype.m_entitiesNum -= 1;
  if (archetype.m_entitiesNum == 0)
  {
    removeArchetype(archetype);
  }

  size_t oldRow = entityRec.row.value();

  Archetype_ID newArchetypeID;
  if (findArchetypeWithComponent(component, newEntityArchetypeType, newArchetypeID))
  {
    Archetype& newArchetype = getArchetypeFromID(newArchetypeID);

    Archetype_Record& newArchetypeRec = m_componentToArchetypeRecord[component][newArchetype.m_id];
    
    size_t row = 0u;
    if (newArchetype.m_freeRow.empty())
    {
      row = newArchetype.m_entitiesNum;
      for (auto& column : newArchetype.m_components)
      {
        column.elements.resize(column.elements.size() + m_componentToElementSize[component]);
      }
    }
    else
    {
      row = newArchetype.m_freeRow.front();
      newArchetype.m_freeRow.pop();
    }

    int index = 0u;
    for (int i = 0; i < archetype.m_components.size(); ++i)
    {
      if (i == archetypeRec.column)
        continue;
      auto& oldColumn = archetype.m_components[i];
      auto& newColumn = newArchetype.m_components[index];
      index++;
      
      oldColumn.elementsNum--;
      newColumn.elementsNum++;

      m_componentToMoveFunc[component](
        newColumn.elements.data() + row * m_componentToElementSize[component], 
        oldColumn.elements.data() + oldRow * m_componentToElementSize[component]);
    }

    newArchetype.m_entitiesNum++;

    Record rec;
    rec.row = row;
    rec.archetypeID = newArchetypeID;
    m_entityToRecord[entity] = rec;
  }
  else
  {
    std::vector<uint64> componentsSizes(archetype.m_components.size() - 1u);
    createArchetypeForEntity(entity, newEntityArchetypeType);
    Record& entityNewRec = m_entityToRecord[entity];
    Archetype& newArchetype = getArchetypeFromID(entityNewRec.archetypeID);
    newArchetypeID = newArchetype.m_id;

    for (auto& column : newArchetype.m_components)
    {
      column.elementsNum = 1u;
      column.elements.resize(m_componentToElementSize[column.component]);
    }
    
    size_t row = 0u;
    int index = 0u;
    for (int i = 0; i < archetype.m_components.size(); ++i)
    {
      if (i == archetypeRec.column)
        continue;

      auto& oldColumn = archetype.m_components[i];
      auto& newColumn = newArchetype.m_components[index];
      index++;
      m_componentToMoveFunc[component](
        newColumn.elements.data() + row * m_componentToElementSize[component], 
        oldColumn.elements.data() + oldRow * m_componentToElementSize[component]);
    }

    newArchetype.m_entitiesNum++;

    Record rec;
    rec.row = row;
    rec.archetypeID = newArchetypeID;
    m_entityToRecord[entity] = rec;
  }
}

void World::createArchetypeForEntity(Entity_ID entity, const std::set<Component_ID>& type)
{
  Archetype archertpe;
  archertpe.m_type = type;
  archertpe.m_edges.clear(); // !TODO
  archertpe.m_components.resize(type.size());
  archertpe.m_entitiesNum = 1u;
  archertpe.m_id = m_archetypeIDsManager.generate();
  
  uint64 column = 0u;
  for (auto& archetypeComponent : type)
  {
    Archetype_Record archetypeRec;
    archetypeRec.column = column++;
    m_componentToArchetypeRecord[archetypeComponent][archertpe.m_id] = archetypeRec;
  }

  column = 0u;
  for (auto& archetypeComponent : type)
  {
    archertpe.m_components[column].component = archetypeComponent;
    column++;
  }

  addArchetypeToArray(archertpe);

  Record entityRec;
  entityRec.archetypeID = archertpe.m_id;
  entityRec.row = std::nullopt;
  m_entityToRecord[entity] = entityRec;
}

void World::assigneOrCreateArchetypeForEntity(Entity_ID entity, Component_ID component)
{
  Archetype_ID newArchetypeID;
  if (findArchetypeWithComponent(component, newArchetypeID))
  {
    Archetype& archetype = getArchetypeFromID(newArchetypeID);

    archetype.m_entitiesNum++;

    Record entityRec;
    entityRec.archetypeID = newArchetypeID;
    entityRec.row = std::nullopt;
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

  archetype.m_entitiesNum -= 1;
  if (archetype.m_entitiesNum == 0)
  {
    removeArchetype(archetype);
  }

  Archetype_ID newArchetypeID;
  if (findArchetypeWithComponent(component, newEntityArchetypeType, newArchetypeID))
  {
    Archetype& newArchetype = getArchetypeFromID(newArchetypeID);
    newArchetype.m_entitiesNum++;

    Record rec;
    rec.row = std::nullopt;
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

void World::removeArchetype(Archetype& archetype)
{
  for (auto& archetypeComponent : archetype.m_type)
  {
    m_componentToArchetypeRecord[archetypeComponent].erase(archetype.m_id);
  }

  m_archetypeIDsManager.destroy(archetype.m_id);
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
  // !TODO
  m_entityIDsManager.destroy(id);
}

}