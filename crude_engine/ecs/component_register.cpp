module crude_engine.ecs.component_register;

import crude_engine.core.assert;

namespace crude_engine
{

const Component_Register::Component_Info& Component_Register::getComponentInfo(Component_ID component) const
{
  assert(m_componentIDToInfo.find(component) != m_componentIDToInfo.end());
  return m_componentIDToInfo.at(component);
}

}