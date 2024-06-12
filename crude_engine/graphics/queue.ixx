module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.queue;

import crude.core.optional;
import crude.core.std_containers_stack;
import crude.core.std_containers_heap;
import crude.graphics.vulkan_object;

export namespace crude::graphics
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
  explicit Queue(core::uint32 familyIndex, core::uint32 index);
  bool sumbit(const core::span<core::shared_ptr<Command_Buffer>>&  commandBuffers,
              const core::span<VkPipelineStageFlags>&              waitStageMasks = {},
              const core::span<core::shared_ptr<Semaphore>>&       waitSemaphores = {},
              const core::span<core::shared_ptr<Semaphore>>&       signalSemaphores = {},
              const core::Optional<core::shared_ptr<Fence>>&       fence = core::nullopt);
  Queue_Present_Result present(const core::span<core::shared_ptr<Swap_Chain>>&  swapchains,
                               const core::span<core::uint32>&                  imageIndices,
                               const core::span<core::shared_ptr<Semaphore>>&   waitSemaphores = {});
  void waitIdle();
  ~Queue() = default;
private:
  const core::uint32  m_familyIndex;
  const core::uint32  m_index;
};

}
