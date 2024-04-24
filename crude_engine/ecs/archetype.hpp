#pragma once

#include <ecs/alias.hpp>
#include <core/std_containers.hpp>

namespace crude_engine
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
    vector<uint8>  m_elements;
    Component_ID   m_component;
  };

public:
  Archetype(Component_Register* pComponentRegister, Archetype_ID id, const set<Component_ID>& type);
  ~Archetype();

  void clear();

  void increaseEntity(int64 num);
  void reduceEntity(int64 num);
  uint64 getEntityNum() const;
  bool entityEmpty() const;

  uint64 newRow();
  void removeComponentData(uint64 row);
  void copyComponentData(uint64 column, uint64 row, const void* value);
  void moveComponentData(uint64 column, uint64 row, void* value);
  void* getComponentData(uint64 column, uint64 row);
  const void* getComponentData(uint64 column, uint64 row) const;

  const set<Component_ID>& type() const;
  Archetype_ID id() const;

  uint64 getRowsNum() const;
  uint64 getColumnsNum() const;

private:
  void increaseCapacity();
  void initializeRowData(uint64 row);
  void deinitializeRowData(uint64 row);

private:
  Archetype_ID              m_id;
  set<Component_ID>         m_type;
  vector<Archetype_Column>  m_components;
  //std::unordered_map<Column, Archetype_Edge>   m_edges;
  uint64                    m_componentsDataCapacity;
  uint64                    m_componentsDataSize;
  queue<uint64>             m_freeRows;
  uint64                    m_entitiesNum;

  Component_Register*       m_pComponentRegister;
};

}