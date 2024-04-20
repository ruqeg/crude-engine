#include <ecs/world.hpp>
#include <ecs/entity.hpp>
#include <core/algorithms.hpp>

namespace crude_engine
{

Entity World::entity()
{
  return Entity(this, newEntityID());
}

Entity_ID World::newEntityID()
{
  Entity_ID id = m_entityIDsManager.generate();
  return id;
}

Entity_ID World::newSystemID()
{
  Entity_ID id = m_systemIDsManager.generate();
  return id;
}

void World::addComponent(Entity_ID entity, Component_ID component)
{
  if (!m_entityToRecord.contains(entity))
  {
    if (!m_componentToArchetypeMap.contains(component))
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
    addComponent(entity, component);
  }

  Entity_Record& entityRecord = m_entityToRecord.at(entity);
  const Archetype_ID archetypeID = entityRecord.archetypeID;
  Archetype& archetype = getArchetype(archetypeID);
  const Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  const uint64 column = componentRecord.at(archetypeID).column;

  const bool noComponentData = !entityRecord.row.hasValue();
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
    CRUDE_ASSERT(false && "TODO");
  }

  Entity_Record& entityRecord = m_entityToRecord.at(entity);
  const bool noComponentData = !entityRecord.row.hasValue();
  if (noComponentData)
  {
    CRUDE_ASSERT(false && "TODO");
  }

  const Archetype_ID archetypeID = entityRecord.archetypeID;
  const Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  Archetype& archetype = getArchetype(archetypeID);
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
  if (!m_entityToRecord.contains(entity))
  {
    return;
  }

  const Entity_Record oldEntityRecord = m_entityToRecord.at(entity);
  const uint64 oldArchetypeID = oldEntityRecord.archetypeID;
  Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  Archetype_Record& oldArchetypeRecord = componentRecord.at(oldEntityRecord.archetypeID);

  std::set<Component_ID> newEntityArchetypeType = getArchetype(oldArchetypeID).type();
  newEntityArchetypeType.erase(component);

  Entity_Record newEntityRecord;
  if (findArchetype(component, newEntityArchetypeType, newEntityRecord.archetypeID))
  {
    newEntityRecord = Entity_Record(newEntityRecord.archetypeID);
  }
  else
  {
    createArchetypeForEntity(entity, newEntityArchetypeType);
    newEntityRecord = m_entityToRecord.at(entity);
  }

  if (oldEntityRecord.row.hasValue())
  {
    Archetype& newArchetype = getArchetype(newEntityRecord.archetypeID);
    newEntityRecord.row = newArchetype.newRow();

    const uint64 oldRow = oldEntityRecord.row.value();
    const uint64 newRow = newEntityRecord.row.value();

    const uint64 skippedColumn = oldArchetypeRecord.column;
    moveEntityComponentDataExceptRemoved(skippedColumn, Unsafe_Entity_Record(&getArchetype(oldArchetypeID), oldRow), Unsafe_Entity_Record(&newArchetype, newRow));

    getArchetype(oldArchetypeID).removeComponentData(oldRow);
  }

  m_entityToRecord[entity] = newEntityRecord;

  Archetype& newArchetype = getArchetype(newEntityRecord.archetypeID);
  newArchetype.increaseEntity(1u);

  getArchetype(oldArchetypeID).reduceEntity(1u);
  if (getArchetype(oldArchetypeID).entityEmpty())
  {
    removeArchetype(getArchetype(oldArchetypeID));
  }
}

bool World::hasComponent(Entity_ID entity, Component_ID component) const
{
  if (!m_entityToRecord.contains(entity) || !m_componentToArchetypeMap.contains(component))
    return false;

  const Entity_Record& entityRecord = m_entityToRecord.at(entity);
  const Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  return componentRecord.contains(entityRecord.archetypeID);
}

void World::remove(Entity_ID entity)
{
  // !TODO

  Entity_Record& entityRecord = m_entityToRecord.at(entity);
  Archetype& archetype = getArchetype(entityRecord.archetypeID);
  for (const auto& component : archetype.type())
  {
    removeComponent(entity, component);
  }

  m_entityToRecord.erase(entity);
  m_entityIDsManager.destroy(entity);
}

void World::createArchetypeForEntity(Entity_ID entity, const std::set<Component_ID>& type)
{
  Archetype archertype(&m_componentRegister, m_archetypeIDsManager.generate(), type);
  const Archetype_ID archertypeID = archertype.id();

  uint64 column = 0u;
  for (auto& component : type)
  {
    Archetype_Record archetypeRecord;
    archetypeRecord.column = column++;
    m_componentToArchetypeMap[component][archertype.id()] = archetypeRecord;
  }
  archertype.increaseEntity(1u);

  addArchetype(archertype);

  Entity_Record entityRecord;
  entityRecord.archetypeID = archertypeID;
  entityRecord.row = nullopt;
  m_entityToRecord[entity] = entityRecord;
}

void World::assigneOrCreateArchetypeForEntity(Entity_ID entity, Component_ID component)
{
  Archetype_ID archetypeID;
  if (!findArchetype(component, archetypeID))
  {
    createArchetypeForEntity(entity, { component });
    return;
  }

  Archetype& archetype = getArchetype(archetypeID);
  archetype.increaseEntity(1u);

  Entity_Record entityRecord;
  entityRecord.archetypeID = archetypeID;
  entityRecord.row = nullopt;
  m_entityToRecord[entity] = entityRecord;
}

void World::reassigneArchetypeForEntity(Entity_ID entity, Component_ID component)
{
  const Entity_Record oldEntityRecord = m_entityToRecord[entity];
  const Archetype_ID oldArchetypeID = oldEntityRecord.archetypeID;

  std::set<Component_ID> newArchetypeType = getArchetype(oldArchetypeID).type();
  newArchetypeType.insert(component);
  
  Entity_Record newEntityRecord;
  if (findArchetype(component, newArchetypeType, newEntityRecord.archetypeID))
  {
    newEntityRecord = Entity_Record(newEntityRecord.archetypeID);
  }
  else
  {
    createArchetypeForEntity(entity, newArchetypeType);
    newEntityRecord = m_entityToRecord.at(entity);
  }

  if (oldEntityRecord.row.hasValue())
  {
    Archetype& newArchetype = getArchetype(newEntityRecord.archetypeID);
    newEntityRecord.row = newArchetype.newRow();

    const uint64 oldRow = oldEntityRecord.row.value();
    const uint64 newRow = newEntityRecord.row.value();

    const uint64 skippedColumn = m_componentToArchetypeMap.at(component).at(newEntityRecord.archetypeID).column;
    moveEntityComponentDataExceptAdded(
      skippedColumn, 
      Unsafe_Entity_Record(&getArchetype(oldArchetypeID), oldRow), 
      Unsafe_Entity_Record(&newArchetype, newRow));

    getArchetype(oldArchetypeID).removeComponentData(oldRow);
  }

  m_entityToRecord[entity] = newEntityRecord;

  Archetype& newArchetype = getArchetype(newEntityRecord.archetypeID);
  newArchetype.increaseEntity(1u);

  getArchetype(oldArchetypeID).reduceEntity(1u);
  if (getArchetype(oldArchetypeID).entityEmpty())
  {
    removeArchetype(getArchetype(oldArchetypeID));
  }
}

bool World::findArchetype(Component_ID component, const std::set<Component_ID>& type, Archetype_ID& dstArchetypeID)
{
  if (!m_componentToArchetypeMap.contains(component))
  {
    return false;
  }

  const Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  for (const auto& archetypeRecord : componentRecord)
  {
    const uint64 archetypeID = archetypeRecord.first;
    const Archetype& archetype = getArchetype(archetypeID);
    if (archetype.type() == type)
    {
      dstArchetypeID = archetypeID;
      return true;
    }
  }
  return false;
}

bool World::findArchetype(Component_ID component, Archetype_ID& dstArchetypeID)
{
  if (!m_componentToArchetypeMap.contains(component))
  {
    return false;
  }

  const Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  for (const auto& archetypeRecord : componentRecord)
  {
    const uint64 archetypeID = archetypeRecord.first;
    Archetype& archetype = getArchetype(archetypeRecord.first);
    if (archetype.type().size() == 1u)
    {
      dstArchetypeID = archetypeID;
      return true;
    }
  }
  return false;
}

void World::addArchetype(Archetype& archetype)
{
  // !TODO UNSAFE
  const ID_Index archetypeIndex = ID_Manager::getIndex(archetype.id());
  CRUDE_ASSERT(archetypeIndex <= m_archetypes.size() && "!TODO");

  if (archetypeIndex < m_archetypes.size())
  {
    m_archetypes[archetypeIndex] = archetype;
  }
  else
  {
    m_archetypes.push_back(archetype);
  }
}

void World::removeArchetype(Archetype& archetype)
{
  archetype.clear();

  for (auto& component : archetype.type())
  {
    m_componentToArchetypeMap.at(component).erase(archetype.id());
  }

  m_archetypeIDsManager.destroy(archetype.id());
}

Archetype& World::getArchetype(const Archetype_ID archetypeID)
{
  return m_archetypes[ID_Manager::getIndex(archetypeID)];
}

const Archetype& World::getArchetype(const Archetype_ID archetypeID) const
{
  return m_archetypes[ID_Manager::getIndex(archetypeID)];
}

void World::moveEntityComponentDataExceptAdded(uint32 skippedColumn, const Unsafe_Entity_Record& srcRecord, const Unsafe_Entity_Record& dstRecord)
{
  const uint64 dstColumnsNum = dstRecord.pArchetype->getColumnsNum();

  for (uint64 newColumn = 0u, oldColumn = 0u; newColumn < dstColumnsNum; ++newColumn)
  {
    if (newColumn == skippedColumn)
      continue;

    dstRecord.pArchetype->moveComponentData(newColumn, dstRecord.row, srcRecord.pArchetype->getComponentData(oldColumn, srcRecord.row));
    oldColumn++;
  }
}

void World::moveEntityComponentDataExceptRemoved(uint32 skippedColumn, const Unsafe_Entity_Record& srcRecord, const Unsafe_Entity_Record& dstRecord)
{
  const uint64 srcColumnsNum = srcRecord.pArchetype->getColumnsNum();

  for (uint64 oldColumn = 0u, newColumn = 0u; oldColumn < srcColumnsNum; ++oldColumn)
  {
    if (oldColumn == skippedColumn)
      continue;

    dstRecord.pArchetype->moveComponentData(newColumn, dstRecord.row, srcRecord.pArchetype->getComponentData(oldColumn, srcRecord.row));
    newColumn++;
  }
}

}