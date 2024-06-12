#include <vector>
#include <set>

module crude.ecs.world;

import crude.core.algorithms;
import crude.core.assert;
import crude.ecs.entity;

namespace crude::ecs
{

World::World()
{
  m_componentRegister = core::allocateShared<Component_Register>();
}

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
  const core::uint64 column = componentRecord.at(archetypeID).column;

  const bool noComponentData = !entityRecord.row.hasValue();
  if (noComponentData)
  {
    entityRecord.row = archetype.newRow();
  }

  const core::uint64 row = entityRecord.row.value();
  archetype.copyComponentData(column, row, value);
}

void* World::getComponent(Entity_ID entity, Component_ID component)
{
  if (!hasComponent(entity, component))
  {
    core::assert(false && "TODO");
  }

  Entity_Record& entityRecord = m_entityToRecord.at(entity);
  const bool noComponentData = !entityRecord.row.hasValue();
  if (noComponentData)
  {
    core::assert(false && "TODO");
  }

  const Archetype_ID archetypeID = entityRecord.archetypeID;
  const Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  Archetype& archetype = getArchetype(archetypeID);
  const core::uint64 column = componentRecord.at(archetypeID).column;
  const core::uint64 row = entityRecord.row.value();
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
  const core::uint64 oldArchetypeID = oldEntityRecord.archetypeID;
  Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  Archetype_Record& oldArchetypeRecord = componentRecord.at(oldEntityRecord.archetypeID);

  core::set<Component_ID> newEntityArchetypeType = getArchetype(oldArchetypeID).type();
  newEntityArchetypeType.erase(component);

  Entity_Record newEntityRecord;
  if (findArchetype(newEntityArchetypeType, newEntityRecord.archetypeID))
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

    const core::uint64 oldRow = oldEntityRecord.row.value();
    const core::uint64 newRow = newEntityRecord.row.value();

    const core::uint64 skippedColumn = oldArchetypeRecord.column;
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

void World::createArchetypeForEntity(Entity_ID entity, const core::set<Component_ID>& type)
{
  Archetype archertype(m_componentRegister, m_archetypeIDsManager.generate(), type);
  const Archetype_ID archertypeID = archertype.id();

  core::uint64 column = 0u;
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
  entityRecord.row = core::nullopt;
  m_entityToRecord[entity] = entityRecord;
}

void World::assigneOrCreateArchetypeForEntity(Entity_ID entity, Component_ID component)
{
  Archetype_ID archetypeID;
  if (!findArchetypeWithComponent(component, archetypeID))
  {
    createArchetypeForEntity(entity, { component });
    return;
  }

  Archetype& archetype = getArchetype(archetypeID);
  archetype.increaseEntity(1u);

  Entity_Record entityRecord;
  entityRecord.archetypeID = archetypeID;
  entityRecord.row = core::nullopt;
  m_entityToRecord[entity] = entityRecord;
}

void World::reassigneArchetypeForEntity(Entity_ID entity, Component_ID component)
{
  const Entity_Record oldEntityRecord = m_entityToRecord[entity];
  const Archetype_ID oldArchetypeID = oldEntityRecord.archetypeID;

  core::set<Component_ID> newArchetypeType = getArchetype(oldArchetypeID).type();
  newArchetypeType.insert(component);
  
  Entity_Record newEntityRecord;
  if (findArchetypeWithComponent(component, newArchetypeType, newEntityRecord.archetypeID))
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
    return;

    const core::uint64 oldRow = oldEntityRecord.row.value();
    const core::uint64 newRow = newEntityRecord.row.value();

    const core::uint64 skippedColumn = m_componentToArchetypeMap.at(component).at(newEntityRecord.archetypeID).column;
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

bool World::findArchetypeWithComponent(Component_ID component, const core::set<Component_ID>& type, Archetype_ID& dstArchetypeID)
{
  if (!m_componentToArchetypeMap.contains(component))
  {
    return false;
  }

  const Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  for (const auto& archetypeRecord : componentRecord)
  {
    const core::uint64 archetypeID = archetypeRecord.first;
    const Archetype& archetype = getArchetype(archetypeID);
    if (archetype.type() == type)
    {
      dstArchetypeID = archetypeID;
      return true;
    }
  }
  return false;
}

bool World::findArchetypeWithComponent(Component_ID component, Archetype_ID& dstArchetypeID)
{
  if (!m_componentToArchetypeMap.contains(component))
  {
    return false;
  }

  const Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  for (const auto& archetypeRecord : componentRecord)
  {
    const core::uint64 archetypeID = archetypeRecord.first;
    Archetype& archetype = getArchetype(archetypeRecord.first);
    if (archetype.type().size() == 1u)
    {
      dstArchetypeID = archetypeID;
      return true;
    }
  }
  return false;
}

bool World::findArchetype(const core::set<Component_ID>& type, Archetype_ID& dstArchetypeID)
{
  for (const auto& archetype : m_archetypes)
  {
    if (archetype.type() == type)
    {
      dstArchetypeID = archetype.id();
      return true;
    }
  }
  return false;
}

void World::addArchetype(Archetype& archetype)
{
  // !TODO UNSAFE
  const ID_Index archetypeIndex = ID_Manager::getIndex(archetype.id());
  core::assert(archetypeIndex <= m_archetypes.size() && "!TODO");

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

void World::moveEntityComponentDataExceptAdded(core::uint32 skippedColumn, const Unsafe_Entity_Record& srcRecord, const Unsafe_Entity_Record& dstRecord)
{
  const core::uint64 dstColumnsNum = dstRecord.pArchetype->getColumnsNum();

  for (core::uint64 newColumn = 0u, oldColumn = 0u; newColumn < dstColumnsNum; ++newColumn)
  {
    if (newColumn == skippedColumn)
      continue;

    dstRecord.pArchetype->moveComponentData(newColumn, dstRecord.row, srcRecord.pArchetype->getComponentData(oldColumn, srcRecord.row));
    oldColumn++;
  }
}

void World::moveEntityComponentDataExceptRemoved(core::uint32 skippedColumn, const Unsafe_Entity_Record& srcRecord, const Unsafe_Entity_Record& dstRecord)
{
  const core::uint64 srcColumnsNum = srcRecord.pArchetype->getColumnsNum();

  for (core::uint64 oldColumn = 0u, newColumn = 0u; oldColumn < srcColumnsNum; ++oldColumn)
  {
    if (oldColumn == skippedColumn)
      continue;

    dstRecord.pArchetype->moveComponentData(newColumn, dstRecord.row, srcRecord.pArchetype->getComponentData(oldColumn, srcRecord.row));
    newColumn++;
  }
}

}