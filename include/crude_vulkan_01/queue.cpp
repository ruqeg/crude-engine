#include "queue.hpp"

namespace crude_vulkan_01
{
  
Queue_Create_Info::Queue_Create_Info(uint32 familyIndex, uint32 index)
  :
  familyIndex(familyIndex),
  index(index)
{}
  
Queue::Queue(const Queue_Create_Info& createInfo)
  :
  m_familyIndex(createInfo.familyIndex),
  m_index(createInfo.index)
{}

}
