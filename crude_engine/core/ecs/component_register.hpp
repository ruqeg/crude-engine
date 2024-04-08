#pragma once

#include <core/debug/assert.hpp>
#include <core/ecs/alias.hpp>
#include <core/utility.hpp>
#include <unordered_map>

namespace crude_engine
{

class Component_Register
{
public:
  using Create_Functinon   = void (*)(void* self);
  using Destroy_Functinon  = void (*)(void* self);
  using Move_Functinon     = void (*)(void* self, void* other);
  using Copy_Functinon     = void (*)(void* self, const void* other);

public:
  struct Component_Info
  {
    Create_Functinon   fnCreate;
    Destroy_Functinon  fnDestroy;
    Move_Functinon     fnMove;
    Copy_Functinon     fnCopy;
    uint64             bsize;
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