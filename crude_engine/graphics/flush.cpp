#include <vulkan/vulkan.hpp>

module crude.graphics.flush;

import crude.graphics.command_buffer;
import crude.graphics.command_pool;
import crude.graphics.queue;
import crude.graphics.device;
import crude.core.logger;

namespace crude::graphics
{

void flush(core::shared_ptr<Command_Buffer> commandBuffer)
{
  const core::uint32 queueFamilyIndex = commandBuffer->getCommandPool()->getQueueFamilyIndex();
  core::shared_ptr<Queue> queue = commandBuffer->getDevice()->getQueueByFamily(queueFamilyIndex, 0u).valueOr(nullptr);
  if (queue.get() == nullptr)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to get queue by index %i in crude::graphics::flush", queueFamilyIndex);
  }

  queue->sumbit(core::span(&commandBuffer, 1u));
  queue->waitIdle();
}

}