#include <ecs/component_container.hpp>

namespace crude_engine
{

template<class Component>
void Default_Component_Container<Component>::create(void* self)
{
  Utility::constructAt<Component>(reinterpret_cast<Component*>(self));
}

template<class Component>
void Default_Component_Container<Component>::destroy(void* self)
{
  Utility::destructorAt<Component>(reinterpret_cast<Component*>(self));
}

template<class Component>
void Default_Component_Container<Component>::move(void* self, void* other)
{
  *reinterpret_cast<Component*>(self) = Utility::move(*reinterpret_cast<Component*>(other));
}

template<class Component>
void Default_Component_Container<Component>::copy(void* self, const void* other)
{
  *reinterpret_cast<Component*>(self) = *reinterpret_cast<const Component*>(other);
}

}