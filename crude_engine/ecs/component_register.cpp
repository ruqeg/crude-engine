module crude.ecs.component_register;

import crude.core.assert;

namespace crude::ecs
{

const Component_Register::Component_Info& Component_Register::getComponentInfo(Component_ID component) const
{
  core::assert(m_componentIDToInfo.find(component) != m_componentIDToInfo.end());
  return m_componentIDToInfo.at(component);
}

}