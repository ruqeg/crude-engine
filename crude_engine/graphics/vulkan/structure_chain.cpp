#include <vulkan/vulkan.h>

module crude.gfx.vk.structure_chain;

namespace crude::gfx::vk
{

Structure_Chain::Structure_Chain()
  : m_tail{ nullptr }
  , m_head{ nullptr }
{}

VkBaseOutStructure* Structure_Chain::getHeadNode()
{
  return m_head;
}

const VkBaseOutStructure* Structure_Chain::getHeadNode() const
{
  return m_head;
}

void Structure_Chain::linkNodeBase(VkBaseOutStructure* baseNode)
{
  if (!m_head)
  {
    m_head = m_tail = baseNode;
  }
  else
  {
    m_tail->pNext = baseNode;
    m_tail = baseNode;
  }
}

}
