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
  Archetype_Map& archetypeMap = m_componentToArchetypeRecord[component];
  Archetype& archetype = getArchetypeFromID(entityRec.archetypeID);
  
  if (!entityRec.row.has_value())
  {
    entityRec.row = archetype.newRow();
  }

  archetype.copyComponentData(archetypeMap[entityRec.archetypeID].column, entityRec.row.value(), value);
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
  uint64 column = componentRec[entityRec.archetypeID].column;
  uint64 row = entityRec.row.value();
  return archetype.getComponentData(column, row);
}

const void* World::getComponent(Entity_ID entity, Component_ID component) const
{
  if (!hasComponent(entity, component))
  {
    // !TODO
    CRUDE_ASSERT(false && "TODO");
  }

  const Record& entityRec = m_entityToRecord.at(entity);
  const Archetype_Map& componentRec = m_componentToArchetypeRecord.at(component);
  const Archetype& archetype = getArchetypeFromID(entityRec.archetypeID);
  uint64 column = componentRec.at(entityRec.archetypeID).column;
  uint64 row = entityRec.row.value();
  return archetype.getComponentData(column, row);
}

void World::removeComponent(Entity_ID entity, Component_ID component)
{
  // no archetype for entity
  if (m_entityToRecord.find(entity) == m_entityToRecord.end())
  {
    return;
  }

  Record entityRec = m_entityToRecord[entity];
  Archetype& archetype = getArchetypeFromID(entityRec.archetypeID);
  Archetype_Record& archetypeRec = m_componentToArchetypeRecord[component][entityRec.archetypeID];

  std::set<Component_ID> newEntityArchetypeType = archetype.getType();
  newEntityArchetypeType.erase(component);

  archetype.reduceEntity(1u);
  if (archetype.entityEmpty())
  {
    removeArchetype(archetype);
  }

  Archetype_ID newArchetypeID;
  if (findArchetypeWithComponent(component, newEntityArchetypeType, newArchetypeID))
  {
    Archetype& newArchetype = getArchetypeFromID(newArchetypeID);

    Archetype_Record& newArchetypeRec = m_componentToArchetypeRecord[component][newArchetype.getID()];

    newArchetype.increaseEntity(1u);

    if (entityRec.row.has_value())
    {
      size_t oldRow = entityRec.row.value();
      const uint64 row = newArchetype.newRow();
      uint64 newColumn = 0u;
      for (uint64 oldColumn = 0u; oldColumn < archetype.getComponentsNum(); ++oldColumn)
      {
        if (oldColumn == archetypeRec.column)
          continue;

        newArchetype.moveComponentData(newColumn, row, archetype.getComponentData(oldColumn, oldRow));
        newColumn++;
      }
      archetype.removeComponentData(oldRow);
      entityRec.row = row;
    }

    Record rec;
    rec.row = entityRec.row;
    rec.archetypeID = newArchetypeID;
    m_entityToRecord[entity] = rec;
  }
  else
  {
    createArchetypeForEntity(entity, newEntityArchetypeType);
    Record& entityNewRec = m_entityToRecord[entity];
    Archetype& newArchetype = getArchetypeFromID(entityNewRec.archetypeID);
    newArchetypeID = newArchetype.getID();

    newArchetype.increaseEntity(1u);

    if (entityRec.row.has_value())
    {
      size_t oldRow = entityRec.row.value();
      const uint64 row = newArchetype.newRow();
      uint64 newColumn = 0u;
      for (uint64 oldColumn = 0u; oldColumn < archetype.getComponentsNum(); ++oldColumn)
      {
        if (oldColumn == archetypeRec.column)
          continue;

        newArchetype.moveComponentData(newColumn, row, archetype.getComponentData(oldColumn, oldRow));
        newColumn++;
      }
      archetype.removeComponentData(oldRow);
      entityRec.row = row;
    }

    Record rec;
    rec.row = entityRec.row;
    rec.archetypeID = newArchetypeID;
    m_entityToRecord[entity] = rec;
  }
}

void World::createArchetypeForEntity(Entity_ID entity, const std::set<Component_ID>& type)
{
  Archetype archertype(this, m_archetypeIDsManager.generate(), type);
  
  uint64 column = 0u;
  for (auto& archetypeComponent : type)
  {
    Archetype_Record archetypeRec;
    archetypeRec.column = column++;
    m_componentToArchetypeRecord[archetypeComponent][archertype.getID()] = archetypeRec;
  }
  archertype.increaseEntity(1u);

  addArchetypeToArray(archertype);

  Record entityRec;
  entityRec.archetypeID = archertype.getID();
  entityRec.row = std::nullopt;
  m_entityToRecord[entity] = entityRec;
}

void World::assigneOrCreateArchetypeForEntity(Entity_ID entity, Component_ID component)
{
  Archetype_ID newArchetypeID;
  if (findArchetypeWithComponent(component, newArchetypeID))
  {
    Archetype& archetype = getArchetypeFromID(newArchetypeID);
    archetype.increaseEntity(1u);

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

  std::set<Component_ID> newEntityArchetypeType = archetype.getType();
  newEntityArchetypeType.insert(component);

  archetype.reduceEntity(1u);
  if (archetype.entityEmpty())
  {
    removeArchetype(archetype);
  }

  Archetype_ID newArchetypeID;
  if (findArchetypeWithComponent(component, newEntityArchetypeType, newArchetypeID))
  {
    Archetype& newArchetype = getArchetypeFromID(newArchetypeID);
    newArchetype.increaseEntity(1u);

    if (record.row.has_value())
    {
      size_t oldRow = record.row.value();
      const uint64 row = newArchetype.newRow();
      uint64 oldColumn = 0u;
      uint64 newCompColumn = m_componentToArchetypeRecord[component][newArchetypeID].column;
      for (uint64 newColumn = 0u; newColumn < archetype.getComponentsNum(); ++newColumn)
      {
        if (newColumn == newCompColumn)
          continue;

        newArchetype.moveComponentData(newColumn, row, archetype.getComponentData(oldColumn, oldRow));
        oldColumn++;
      }
      archetype.removeComponentData(oldRow);

      record.row = row;
    }

    Record rec;
    rec.row = record.row;
    rec.archetypeID = newArchetypeID;
    m_entityToRecord[entity] = rec;
  }
  else
  {
    createArchetypeForEntity(entity, newEntityArchetypeType);

    Record& entityNewRec = m_entityToRecord[entity];
    Archetype& newArchetype = getArchetypeFromID(entityNewRec.archetypeID);
    newArchetypeID = newArchetype.getID();

    newArchetype.increaseEntity(1u);

    if (record.row.has_value())
    {
      size_t oldRow = record.row.value();
      const uint64 row = newArchetype.newRow();
      uint64 oldColumn = 0u;
      uint64 newCompColumn = m_componentToArchetypeRecord[component][newArchetypeID].column;
      for (uint64 newColumn = 0u; newColumn < archetype.getComponentsNum(); ++newColumn)
      {
        if (newColumn == newCompColumn)
          continue;

        newArchetype.moveComponentData(newColumn, row, archetype.getComponentData(oldColumn, oldRow));
        oldColumn++;
      }
      archetype.removeComponentData(oldRow);
      record.row = row;
    }

    Record rec;
    rec.row = record.row;
    rec.archetypeID = newArchetypeID;
    m_entityToRecord[entity] = rec;
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
      if (archetype.getType() == type)
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
      if (archetype.getType().size() == 1)
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
  for (auto& component : archetype.getType())
  {
    m_componentToArchetypeRecord[component].erase(archetype.getID());
  }

  m_archetypeIDsManager.destroy(archetype.getID());
}

void World::addArchetypeToArray(const Archetype& archetype)
{
  // !UNSAFE
  ID_Index index = ID_Manager::getIndex(archetype.getID());
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

const Archetype& World::getArchetypeFromID(const Archetype_ID archetypeID) const
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