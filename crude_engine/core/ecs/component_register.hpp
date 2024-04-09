#pragma once

#include <core/ecs/component_container.hpp>
#include <core/debug/assert.hpp>
#include <core/utility.hpp>
#include <unordered_map>

namespace crude_engine
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
  std::unordered_map<Component_ID, Component_Info> m_componentIDToInfo;
};

}

#include <core/ecs/component_register.inl>