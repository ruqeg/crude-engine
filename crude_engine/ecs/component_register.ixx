module;

export module crude.ecs.component_register;

import crude.ecs.alias;
import crude.ecs.component_container;
import crude.core.std_containers_heap;
import crude.core.assert;
import crude.core.utility;

export namespace crude::ecs
{

class Component_Register
{
public:
  struct Component_Info
  {
    Component_Container::Create_Functinon   fnCreate;
    Component_Container::Destroy_Functinon  fnDestroy;
    Component_Container::Move_Functinon     fnMove;
    Component_Container::Copy_Functinon     fnCopy;
    core::uint64                            bsize;
  };

public:
  template<class Component>
  void registerComponent();

  const Component_Info& getComponentInfo(Component_ID component) const;

private:
  core::unordered_map<Component_ID, Component_Info> m_componentIDToInfo;
};

template<class Component>
void Component_Register::registerComponent()
{
  const Component_ID component = core::CPP_Type<Component>::id();

  Component_Info info;
  info.bsize     = sizeof(Component);
  info.fnCopy    = Component::copy;
  info.fnCreate  = Component::create;
  info.fnDestroy = Component::destroy;
  info.fnMove    = Component::move;

  m_componentIDToInfo[component] = info;
}

}