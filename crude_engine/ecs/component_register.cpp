#include <ecs/component_register.hpp>

namespace crude_engine
{

const Component_Register::Component_Info& Component_Register::getComponentInfo(Component_ID component) const
{
  CRUDE_ASSERT(m_componentIDToInfo.find(component) != m_componentIDToInfo.end());
  return m_componentIDToInfo.at(component);
}

}