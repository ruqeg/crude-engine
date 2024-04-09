#pragma once

#include <core/ecs/alias.hpp>
#include <core/utility.hpp>

namespace crude_engine
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

}

#include <core/ecs/component_container.inl>