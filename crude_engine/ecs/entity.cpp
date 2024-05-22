module crude.ecs.entity;

namespace crude::ecs
{

Entity::Entity()
{
  m_world = nullptr;
}

Entity::Entity(World* world)
{
  m_world = world;
  m_id = world->newEntityID();
}

Entity::Entity(const World* world, Entity_ID id)
{
  m_world = const_cast<World*>(world);
  m_id = id;
}

Entity_ID Entity::id() const
{
  return m_id;
}

void Entity::destruct()
{
  m_world->remove(m_id);
}

} //namespace crude