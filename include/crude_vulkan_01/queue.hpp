#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include<vector>

namespace crude_vulkan_01
{

class Device;
class Command_Buffer;

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
  bool sumbit(const std::vector<std::shared_ptr<Command_Buffer>>& commandBuffers);
  void waitIdle();
  ~Queue() = default;
private:
  const uint32  m_familyIndex;
  const uint32  m_index;
};

}
