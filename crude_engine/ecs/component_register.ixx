module;

export module crude_engine.ecs.component_register;

import crude_engine.ecs.alias;
import crude_engine.ecs.component_container;
import crude_engine.core.std_containers_heap;
import crude_engine.core.assert;
import crude_engine.core.utility;

export namespace crude_engine
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
    uint64                                  bsize;
  };

public:
  template<class Component>
  void registerComponent();

  const Component_Info& getComponentInfo(Component_ID component) const;

private:
  unordered_map<Component_ID, Component_Info> m_componentIDToInfo;
};

template<class Component>
void Component_Register::registerComponent()
{
  const Component_ID component = CPP_Type<Component>::id();

  Component_Info info;
  info.bsize     = sizeof(Component);
  info.fnCopy    = Component::copy;
  info.fnCreate  = Component::create;
  info.fnDestroy = Component::destroy;
  info.fnMove    = Component::move;

  m_componentIDToInfo[component] = info;
}

}