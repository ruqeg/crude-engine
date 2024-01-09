#pragma once

#include <core/data_structures/shared_ptr.hpp>
#include <core/data_structures/array_unsafe.hpp>
#include <core/data_structures/optional.hpp>
#include <graphics/vulkan/object.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
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

class Queue : public TObject<VkQueue>
{
public:
  explicit Queue(uint32 familyIndex, uint32 index);
  bool sumbit(const Array_Unsafe<Shared_Ptr<Command_Buffer>>&  commandBuffers, 
              const Array_Unsafe<VkPipelineStageFlags>&        waitStageMasks = {}, 
              const Array_Unsafe<Shared_Ptr<Semaphore>>&       waitSemaphores = {},
              const Array_Unsafe<Shared_Ptr<Semaphore>>&       signalSemaphores = {},
              const Optional<Shared_Ptr<Fence>>&               fence = nullopt);
  Queue_Present_Result present(const Array_Unsafe<Shared_Ptr<Swap_Chain>>&  swapchains,
                               const Array_Unsafe<uint32>&                  imageIndices,
                               const Array_Unsafe<Shared_Ptr<Semaphore>>&   waitSemaphores = {});
  void waitIdle();
  ~Queue() = default;
private:
  const uint32  m_familyIndex;
  const uint32  m_index;
};

}
