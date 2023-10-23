#include "queue.hpp"

namespace crude_vulkan_01
{
  
QueueCreateInfo::QueueCreateInfo(uint32 familyIndex, uint32 index)
  :
  familyIndex(familyIndex),
  index(index)
{}
  
Queue::Queue(const QueueCreateInfo& createInfo)
  :
  m_familyIndex(createInfo.familyIndex),
  m_index(createInfo.index)
{}

}
