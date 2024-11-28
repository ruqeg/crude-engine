#include <vulkan/vulkan.hpp>

module crude.gfx.vk.flush;

import crude.gfx.vk.command_buffer;
import crude.gfx.vk.command_pool;
import crude.gfx.vk.queue;
import crude.gfx.vk.device;
import crude.core.logger;

namespace crude::gfx::vk
{

void flush(core::shared_ptr<Command_Buffer> commandBuffer)
{
  const core::uint32 queueFamilyIndex = commandBuffer->getCommandPool()->getQueueFamilyIndex();
  core::shared_ptr<Queue> queue = commandBuffer->getDevice()->getQueueByFamily(queueFamilyIndex, 0u).value_or(nullptr);
  if (queue.get() == nullptr)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed to get queue by index %i in crude::gfx::vk::flush", queueFamilyIndex);
  }

  queue->sumbit(core::span(&commandBuffer, 1u));
  queue->waitIdle();
}

}