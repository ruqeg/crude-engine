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
  const bool noEntityRecord = (m_entityToRecord.find(entity) == m_entityToRecord.end());
  if (noEntityRecord)
  {
    const bool noArchetypeWithComponent = (m_componentToArchetypeMap.find(component) == m_componentToArchetypeMap.end());
    if (noArchetypeWithComponent)
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

  Entity_Record& entityRecord = m_entityToRecord.at(entity);
  const Archetype_ID archetypeID = entityRecord.archetypeID;
  Archetype& archetype = getArchetypeFromID(archetypeID);
  const Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  const uint64 column = componentRecord.at(archetypeID).column;

  const bool noComponentData = !entityRecord.row.has_value();
  if (noComponentData)
  {
    entityRecord.row = archetype.newRow();
  }

  const uint64 row = entityRecord.row.value();
  archetype.copyComponentData(column, row, value);
}

void* World::getComponent(Entity_ID entity, Component_ID component)
{
  if (!hasComponent(entity, component))
  {
    // !TODO
    CRUDE_ASSERT(false && "TODO");
  }

  Entity_Record& entityRecord = m_entityToRecord.at(entity);
  const bool noComponentData = !entityRecord.row.has_value();
  if (noComponentData)
  {
    // !TODO
    CRUDE_ASSERT(false && "TODO");
  }

  const Archetype_ID archetypeID = entityRecord.archetypeID;
  const Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  Archetype& archetype = getArchetypeFromID(archetypeID);
  const uint64 column = componentRecord.at(archetypeID).column;
  const uint64 row = entityRecord.row.value();
  void* componentData = archetype.getComponentData(column, row);
  return componentData;
}

const void* World::getComponent(Entity_ID entity, Component_ID component) const
{
  return const_cast<const void*>(getComponent(entity, component));
}

void World::removeComponent(Entity_ID entity, Component_ID component)
{
  // no archetype for entity
  const bool noEntityRecord = (m_entityToRecord.find(entity) == m_entityToRecord.end());
  if (noEntityRecord)
  {
    return;
  }

  Entity_Record entityRecord = m_entityToRecord.at(entity);
  const uint64 archetypeID = entityRecord.archetypeID;
  Archetype& archetype = getArchetypeFromID(archetypeID);
  Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  Archetype_Record& archetypeRecord = componentRecord.at(entityRecord.archetypeID);

  std::set<Component_ID> newEntityArchetypeType = archetype.getType();
  newEntityArchetypeType.erase(component);

  archetype.reduceEntity(1u);
  if (archetype.entityEmpty())
    removeArchetype(archetype);

  Archetype_ID newArchetypeID;
  if (findArchetype(component, newEntityArchetypeType, newArchetypeID))
  {
    Archetype& newArchetype = getArchetypeFromID(newArchetypeID);
    Archetype_Record& newArchetypeRec = componentRecord.at(newArchetypeID);
    newArchetype.increaseEntity(1u);

    Entity_Record newEntityRecord;
    if (entityRecord.row.has_value())
    {
      newEntityRecord.row = moveComponentDataToArchetype(entityRecord.row.value(), archetypeRecord.column, archetype, newArchetype);
    }
    newEntityRecord.archetypeID = newArchetypeID;
    m_entityToRecord[entity] = newEntityRecord;
  }
  else
  {
    createArchetypeForEntity(entity, newEntityArchetypeType);
    Entity_Record& newEntityRecord = m_entityToRecord.at(entity);
    Archetype& newArchetype = getArchetypeFromID(newEntityRecord.archetypeID);
    newArchetypeID = newArchetype.getID();

    newArchetype.increaseEntity(1u);

    if (entityRecord.row.has_value())
    {
      newEntityRecord.row = moveComponentDataToArchetype(entityRecord.row.value(), archetypeRecord.column, archetype, newArchetype);
    }
    newEntityRecord.archetypeID = newArchetypeID;
  }
}

void World::createArchetypeForEntity(Entity_ID entity, const std::set<Component_ID>& type)
{
  Archetype archertype(&m_componentRegister, m_archetypeIDsManager.generate(), type);
  
  uint64 column = 0u;
  for (auto& archetypeComponent : type)
  {
    Archetype_Record archetypeRec;
    archetypeRec.column = column++;
    m_componentToArchetypeMap[archetypeComponent][archertype.getID()] = archetypeRec;
  }
  archertype.increaseEntity(1u);

  addArchetypeToArray(archertype);

  Entity_Record entityRec;
  entityRec.archetypeID = archertype.getID();
  entityRec.row = std::nullopt;
  m_entityToRecord[entity] = entityRec;
}

void World::assigneOrCreateArchetypeForEntity(Entity_ID entity, Component_ID component)
{
  Archetype_ID newArchetypeID;
  if (findArchetype(component, newArchetypeID))
  {
    Archetype& archetype = getArchetypeFromID(newArchetypeID);
    archetype.increaseEntity(1u);

    Entity_Record entityRec;
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
  Entity_Record record = m_entityToRecord[entity];
  Archetype& archetype = getArchetypeFromID(record.archetypeID);

  std::set<Component_ID> newEntityArchetypeType = archetype.getType();
  newEntityArchetypeType.insert(component);

  archetype.reduceEntity(1u);
  if (archetype.entityEmpty())
  {
    removeArchetype(archetype);
  }

  Archetype_ID newArchetypeID;
  if (findArchetype(component, newEntityArchetypeType, newArchetypeID))
  {
    Archetype& newArchetype = getArchetypeFromID(newArchetypeID);
    newArchetype.increaseEntity(1u);

    if (record.row.has_value())
    {
      record.row = moveComponentDataToArchetype(
        record.row.value(),
        m_componentToArchetypeMap[component][newArchetypeID].column,
        archetype,
        newArchetype);
    }

    Entity_Record rec;
    rec.row = record.row;
    rec.archetypeID = newArchetypeID;
    m_entityToRecord[entity] = rec;
  }
  else
  {
    createArchetypeForEntity(entity, newEntityArchetypeType);

    Entity_Record& entityNewRec = m_entityToRecord[entity];
    Archetype& newArchetype = getArchetypeFromID(entityNewRec.archetypeID);
    newArchetypeID = newArchetype.getID();

    newArchetype.increaseEntity(1u);

    if (record.row.has_value())
    {
      record.row = moveComponentDataToArchetype(
        record.row.value(),
        m_componentToArchetypeMap[component][newArchetypeID].column,
        archetype,
        newArchetype);
    }

    Entity_Record rec;
    rec.row = record.row;
    rec.archetypeID = newArchetypeID;
    m_entityToRecord[entity] = rec;
  }
}

bool World::findArchetype(Component_ID component, const std::set<Component_ID>& type, Archetype_ID& dstArchetypeID)
{
  const bool noArchetypeWithComponent = (m_componentToArchetypeMap.find(component) == m_componentToArchetypeMap.end());
  if (noArchetypeWithComponent)
  {
    return false;
  }

  const Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  for (const auto& archetypeRecord : componentRecord)
  {
    const uint64 archetypeID = archetypeRecord.first;
    const Archetype& archetype = getArchetypeFromID(archetypeID);
    if (archetype.getType() == type)
    {
      dstArchetypeID = archetypeID;
      return true;
    }
  }
  return false;
}

bool World::findArchetype(Component_ID component, Archetype_ID& dstArchetypeID)
{
  const bool noArchetypeWithComponent = (m_componentToArchetypeMap.find(component) == m_componentToArchetypeMap.end());
  if (noArchetypeWithComponent)
  {
    return false;
  }

  const Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  for (const auto& archetypeRecord : componentRecord)
  {
    const uint64 archetypeID = archetypeRecord.first;
    Archetype& archetype = getArchetypeFromID(archetypeRecord.first);
    if (archetype.getType().size() == 1u)
    {
      dstArchetypeID = archetypeID;
      return true;
    }
  }
  return false;
}

void World::removeArchetype(Archetype& archetype)
{
  for (auto& component : archetype.getType())
  {
    m_componentToArchetypeMap[component].erase(archetype.getID());
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

uint64 World::moveComponentDataToArchetype(uint32 srcRow, uint32 srcSkippedColumn, Archetype& srcArchetype, Archetype& dstArchetype)
{
  const uint64 row = dstArchetype.newRow();
  
  uint64 newColumn = 0u;
  for (uint64 oldColumn = 0u; oldColumn < srcArchetype.getComponentsNum(); ++oldColumn)
  {
    if (oldColumn == srcSkippedColumn)
      continue;

    dstArchetype.moveComponentData(newColumn, row, srcArchetype.getComponentData(oldColumn, srcRow));
    newColumn++;
  }
  srcArchetype.removeComponentData(srcRow);
  return row;
}

bool World::hasComponent(Entity_ID entity, Component_ID component) const
{
  // entity not created or archetype with the component doen't exist 
  if ((m_entityToRecord.find(entity) == m_entityToRecord.end()) || (m_componentToArchetypeMap.find(component) == m_componentToArchetypeMap.end()))
  {
    return false;
  }

  const Entity_Record& record = m_entityToRecord.at(entity);
  const Archetype_Map& archetypeSet = m_componentToArchetypeMap.at(component);
  return (archetypeSet.count(record.archetypeID) != 0);
}

void World::remove(Entity_ID id)
{
  // !TODO
  m_entityIDsManager.destroy(id);
}

}