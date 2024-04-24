#include <ecs/component_register.hpp>

namespace crude_engine
{

template<class Component>
void Component_Register::registerComponent()
{
  const Component_ID component = CPP_Type<Component>::id();

  Component_Info info;
  info.bsize      = sizeof(Component);
  info.fnCopy     = Component::copy;
  info.fnCreate   = Component::create;
  info.fnDestroy  = Component::destroy;
  info.fnMove     = Component::move;

  m_componentIDToInfo[component] = info;
}

}