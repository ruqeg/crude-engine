module;

#include <utility>

export module crude.ecs.component_container;

import crude.ecs.alias;
import crude.core.utility;

export namespace crude::ecs
{

class Component_Container
{
public:
  using Create_Functinon  = void (*)(void* self);
  using Destroy_Functinon = void (*)(void* self);
  using Move_Functinon    = void (*)(void* self, void* other);
  using Copy_Functinon    = void (*)(void* self, const void* other);
};

template<class Component>
class Default_Component_Container : public Component_Container
{
public:
  static void create(void* self);
  static void destroy(void* self);
  static void move(void* self, void* other);
  static void copy(void* self, const void* other);
};

template<class Component>
void Default_Component_Container<Component>::create(void* self)
{
  constructAt<Component>(reinterpret_cast<Component*>(self));
}

template<class Component>
void Default_Component_Container<Component>::destroy(void* self)
{
  destructorAt<Component>(reinterpret_cast<Component*>(self));
}

template<class Component>
void Default_Component_Container<Component>::move(void* self, void* other)
{
  *reinterpret_cast<Component*>(self) = std::move(*reinterpret_cast<Component*>(other));
}

template<class Component>
void Default_Component_Container<Component>::copy(void* self, const void* other)
{
  *reinterpret_cast<Component*>(self) = *reinterpret_cast<const Component*>(other);
}

}