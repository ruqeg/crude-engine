module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.queue;

import crude_engine.core.shared_ptr;
import crude_engine.core.optional;
import crude_engine.core.std_containers_stack;
import crude_engine.graphics.vulkan.vulkan_object;

export namespace crude_engine
{

class Device;
class Command_Buffer;
class Fence;
class Semaphore;
class Swap_Chain;

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

class Queue : public Vulkan_Object<VkQueue>
{
public:
  explicit Queue(uint32 familyIndex, uint32 index);
  bool sumbit(const span<Shared_Ptr<Command_Buffer>>&  commandBuffers,
              const span<VkPipelineStageFlags>&        waitStageMasks = {},
              const span<Shared_Ptr<Semaphore>>&       waitSemaphores = {},
              const span<Shared_Ptr<Semaphore>>&       signalSemaphores = {},
              const Optional<Shared_Ptr<Fence>>&               fence = nullopt);
  Queue_Present_Result present(const span<Shared_Ptr<Swap_Chain>>&  swapchains,
                               const span<uint32>&                  imageIndices,
                               const span<Shared_Ptr<Semaphore>>&   waitSemaphores = {});
  void waitIdle();
  ~Queue() = default;
private:
  const uint32  m_familyIndex;
  const uint32  m_index;
};

}
