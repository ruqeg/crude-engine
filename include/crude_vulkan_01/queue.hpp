#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"

namespace crude_vulkan_01
{

class Device;

struct Queue_Create_Info
{
  uint32  familyIndex;
  uint32  index;

  explicit Queue_Create_Info(uint32 familyIndex, uint32 index);
};

class Queue : public TObject<VkQueue>
{
public:
  explicit Queue(const Queue_Create_Info& createInfo);
  ~Queue() = default;
private:
  const uint32  m_familyIndex;
  const uint32  m_index;
};

}
