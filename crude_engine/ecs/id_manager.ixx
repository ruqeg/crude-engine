module;

export module crude.ecs.id_manager;

import crude.ecs.alias;
import crude.core.std_containers_heap;

export namespace crude::ecs
{

class ID_Manager
{
public:
  ID_Manager();
  
public:
  ID generate();
  void destroy(ID id);

public:
  static ID format(ID_Index index, ID_Generation generation);
  static ID_Generation getGeneration(ID id);
  static ID_Index getIndex(ID id);

private:
  core::queue<ID>  m_freeIDs;
  core::uint64     m_newFreeIndex;
};

}