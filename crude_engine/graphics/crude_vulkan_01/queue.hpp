#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <vector>
#include <optional>

namespace crude_vulkan_01
{

class Device;
class Command_Buffer;
class Fence;
class Semaphore;
class Swap_Chain;

struct Queue_Create_Info
{
  uint32  familyIndex;
  uint32  index;

  explicit Queue_Create_Info(uint32 familyIndex, uint32 index);
};

class Queue_Present_Result
{
public:
  bool outOfDate() const;
  bool suboptimal() const;
  bool failed() const;
  VkResult getResult() const;
private:
  Queue_Present_Result(VkResult result);
private:
  VkResult m_result;

  friend class Queue;
};;

class Queue : public TObject<VkQueue>
{
public:
  explicit Queue(const Queue_Create_Info& createInfo);
  bool sumbit(const std::vector<std::shared_ptr<Command_Buffer>>&  commandBuffers, 
              const std::vector<VkPipelineStageFlags>&             waitStageMasks = {}, 
              const std::vector<std::shared_ptr<Semaphore>>&       waitSemaphores = {}, 
              const std::vector<std::shared_ptr<Semaphore>>&       signalSemaphores = {},
              const std::optional<std::shared_ptr<Fence>>&         fence = std::nullopt);
  Queue_Present_Result present(const std::vector<std::shared_ptr<Swap_Chain>>&  swapchains,
                               const std::vector<uint32>&                       imageIndices, 
                               const std::vector<std::shared_ptr<Semaphore>>&   waitSemaphores = {});
  void waitIdle();
  ~Queue() = default;
private:
  const uint32  m_familyIndex;
  const uint32  m_index;
};

}
