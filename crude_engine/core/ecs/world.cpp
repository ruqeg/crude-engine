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
    // !TODO
    CRUDE_ASSERT(false && "TODO");
    return;
  }

  Entity_Record& entityRecord = m_entityToRecord.at(entity);
  const Archetype_ID archetypeID = entityRecord.archetypeID;
  Archetype& archetype = getArchetype(archetypeID);
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

  const Entity_Record entityRecord = m_entityToRecord.at(entity);
  const uint64 archetypeID = entityRecord.archetypeID;
  Archetype& archetype = getArchetype(archetypeID);
  Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  Archetype_Record& archetypeRecord = componentRecord.at(entityRecord.archetypeID);

  std::set<Component_ID> newEntityArchetypeType = archetype.getType();
  newEntityArchetypeType.erase(component);

  auto fun = [this](Entity_Record& srcRecord, Entity_Record& dstRecord) -> void
  {

  };

  Entity_Record newEntityRecord;
  if (findArchetype(component, newEntityArchetypeType, newEntityRecord.archetypeID))
  {
    Archetype& newArchetype = getArchetype(newEntityRecord.archetypeID);
    newArchetype.increaseEntity(1u);

    if (entityRecord.row.has_value())
    {
      newEntityRecord.row = moveComponentData(entityRecord.row.value(), archetypeRecord.column, archetype, newArchetype);
    }
  }
  else
  {
    createArchetypeForEntity(entity, newEntityArchetypeType);
    newEntityRecord = m_entityToRecord.at(entity);

    Archetype& newArchetype = getArchetype(newEntityRecord.archetypeID);
    newArchetype.increaseEntity(1u);

    if (entityRecord.row.has_value())
    {
      newEntityRecord.row = moveComponentData(entityRecord.row.value(), archetypeRecord.column, archetype, newArchetype);
    }
  }

  m_entityToRecord[entity] = newEntityRecord;

  archetype.reduceEntity(1u);
  if (archetype.entityEmpty())
    removeArchetype(archetype);
}

bool World::hasComponent(Entity_ID entity, Component_ID component) const
{
  if (!m_entityToRecord.contains(entity) || !m_componentToArchetypeMap.contains(component))
    return false;

  const Entity_Record& entityRecord = m_entityToRecord.at(entity);
  const Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  return componentRecord.contains(entityRecord.archetypeID);
}

void World::remove(Entity_ID id)
{
  // !TODO
  m_entityIDsManager.destroy(id);
}

void World::createArchetypeForEntity(Entity_ID entity, const std::set<Component_ID>& type)
{
  Archetype archertype(&m_componentRegister, m_archetypeIDsManager.generate(), type);
  const Archetype_ID archetypeID = archertype.getID();

  uint64 column = 0u;
  for (auto& component : type)
  {
    Archetype_Record archetypeRecord;
    archetypeRecord.column = column++;
    m_componentToArchetypeMap[component][archetypeID] = archetypeRecord;
  }
  archertype.increaseEntity(1u);

  const ID_Index archetypeIndex = ID_Manager::getIndex(archetypeID);
  if (archetypeIndex < m_archetypes.size())
  {
    m_archetypes[archetypeIndex] = archertype;
  }
  else
  {
    m_archetypes.push_back(archertype);
  }

  Entity_Record entityRecord;
  entityRecord.archetypeID = archetypeID;
  entityRecord.row = std::nullopt;
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
  entityRecord.row = std::nullopt;
  m_entityToRecord[entity] = entityRecord;
}

void World::reassigneArchetypeForEntity(Entity_ID entity, Component_ID component)
{
  Entity_Record oldEntityRecord = m_entityToRecord[entity];
  Archetype& archetype = getArchetype(oldEntityRecord.archetypeID);

  std::set<Component_ID> newEntityArchetypeType = archetype.getType();
  newEntityArchetypeType.insert(component);
  
  Entity_Record newEntityRecord;
  if (!findArchetype(component, newEntityArchetypeType, newEntityRecord.archetypeID))
  {
    createArchetypeForEntity(entity, newEntityArchetypeType);
    newEntityRecord = m_entityToRecord.at(entity);
  }

  moveComponentDataExceptAdded(component, oldEntityRecord, newEntityRecord);

  getArchetype(newEntityRecord.archetypeID).increaseEntity(1u);

  m_entityToRecord[entity] = newEntityRecord;

  archetype.reduceEntity(1u);
  if (archetype.entityEmpty())
  {
    removeArchetype(archetype);
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
  if (!m_componentToArchetypeMap.contains(component))
  {
    return false;
  }

  const Archetype_Map& componentRecord = m_componentToArchetypeMap.at(component);
  for (const auto& archetypeRecord : componentRecord)
  {
    const uint64 archetypeID = archetypeRecord.first;
    Archetype& archetype = getArchetype(archetypeRecord.first);
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
  const Archetype_ID archetypeID = archetype.getID();
  for (auto& component : archetype.getType())
  {
    m_componentToArchetypeMap.at(component).erase(archetypeID);
  }

  m_archetypeIDsManager.destroy(archetypeID);
}

Archetype& World::getArchetype(const Archetype_ID archetypeID)
{
  return m_archetypes[ID_Manager::getIndex(archetypeID)];
}

const Archetype& World::getArchetype(const Archetype_ID archetypeID) const
{
  return m_archetypes[ID_Manager::getIndex(archetypeID)];
}
uint64 World::moveComponentData(uint32 srcRow, uint32 srcSkippedColumn, Archetype& srcArchetype, Archetype& dstArchetype)
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

void World::moveComponentDataExceptAdded(Component_ID addedComponent, Entity_Record& srcRecord, Entity_Record& dstRecord)
{
  if (!srcRecord.row.has_value())
  {
    return;
  }

  const Archetype_ID srcArchetypeID = srcRecord.archetypeID;
  const Archetype_ID dstArchetypeID = dstRecord.archetypeID;
  Archetype& srcArchetype = getArchetype(srcArchetypeID);
  Archetype& dstArchetype = getArchetype(dstArchetypeID);

  dstRecord.row = dstArchetype.newRow();

  const uint64 srcRow = srcRecord.row.value();
  const uint64 dstRow = dstRecord.row.value();
  const uint64 skippedColumn = m_componentToArchetypeMap.at(addedComponent).at(dstRecord.archetypeID).column;
  const uint64 srcComponentsNum = srcArchetype.getComponentsNum();

  for (uint64 oldColumn = 0u, newColumn = 0u; oldColumn < srcComponentsNum; ++oldColumn)
  {
    if (oldColumn == skippedColumn)
      continue;

    dstArchetype.moveComponentData(newColumn, dstRow, srcArchetype.getComponentData(oldColumn, srcRow));
    newColumn++;
  }

  srcArchetype.removeComponentData(srcRow);
}

}