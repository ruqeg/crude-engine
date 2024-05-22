module crude.ecs.id_manager;

namespace crude::ecs
{

ID_Manager::ID_Manager()
  :
  m_newFreeIndex(0u)
{}

ID ID_Manager::generate()
{
  ID id;
  if (m_freeIDs.empty())
  {
    id = format(m_newFreeIndex, 0u);
    m_newFreeIndex++;
  }
  else
  {
    id = m_freeIDs.front();
    m_freeIDs.pop();

    id = format(getIndex(id), getGeneration(id) + 1u);
  }
  return id;
}

void ID_Manager::destroy(ID id)
{
  m_freeIDs.push(id);
}

ID ID_Manager::format(ID_Index index, ID_Generation generation)
{
  return (static_cast<ID>(generation) << 48) | static_cast<ID>(index);
}

ID_Generation ID_Manager::getGeneration(ID id)
{
  return (id & 0xFFFF000000000000) >> 48;
}

ID_Index ID_Manager::getIndex(ID id)
{
  return (id & 0x0000FFFFFFFFFFFF);
}

}