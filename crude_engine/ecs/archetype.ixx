module;

export module crude.ecs.archetype;

import crude.ecs.alias;
import crude.core.std_containers_heap;

export namespace crude::ecs
{

class World;
class Component_Register;

class Archetype
{
private:
  struct Archetype_Edge
  {
    Archetype_ID  addID;
    Archetype_ID  removeID;
  };

  struct Archetype_Column
  {
    core::vector<core::uint8>  m_elements;
    Component_ID               m_component;
  };

public:
  Archetype(core::shared_ptr<Component_Register> componentRegister, Archetype_ID id, const core::set<Component_ID>& type);
  ~Archetype();

  void clear();

  void increaseEntity(core::int64 num);
  void reduceEntity(core::int64 num);
  core::uint64 getEntityNum() const;
  bool entityEmpty() const;

  core::uint64 newRow();
  void removeComponentData(core::uint64 row);
  void copyComponentData(core::uint64 column, core::uint64 row, const void* value);
  void moveComponentData(core::uint64 column, core::uint64 row, void* value);
  void* getComponentData(core::uint64 column, core::uint64 row);
  const void* getComponentData(core::uint64 column, core::uint64 row) const;

  const core::set<Component_ID>& type() const;
  Archetype_ID id() const;

  core::uint64 getRowsNum() const;
  core::uint64 getColumnsNum() const;

private:
  void increaseCapacity();
  void initializeRowData(core::uint64 row);
  void deinitializeRowData(core::uint64 row);

private:
  Archetype_ID                          m_id;
  core::set<Component_ID>               m_type;
  core::vector<Archetype_Column>        m_components;
  core::uint64                          m_componentsDataCapacity;
  core::uint64                          m_componentsDataSize;
  core::queue<core::uint64>             m_freeRows;
  core::uint64                          m_entitiesNum;
  core::shared_ptr<Component_Register>  m_componentRegister;

  // !TODO std::unordered_map<Column, Archetype_Edge>   m_edges;
};

}