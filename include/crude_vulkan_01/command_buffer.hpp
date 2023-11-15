#pragma once

#include "image_memory_barrier.hpp"
#include "object.hpp"
#include <vector>

namespace crude_vulkan_01
{

class Device;
class Command_Pool;
class Buffer;
class Image;

struct Command_Buffer_Create_Info
{
  std::shared_ptr<const Device>  device;
  std::shared_ptr<Command_Pool>  commandPool;
  VkCommandBufferLevel           level;
  explicit Command_Buffer_Create_Info(std::shared_ptr<const Device>  device,
                                      std::shared_ptr<Command_Pool>  commandPool,
                                      VkCommandBufferLevel           level);
};


class Command_Buffer : public TObject<VkCommandBuffer>
{
public:
  explicit Command_Buffer(const Command_Buffer_Create_Info& createInfo);
  bool begin(VkCommandBufferUsageFlags flags = 0u);
  bool end();
  void barrier(VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, const std::vector<Image_Memory_Barrier>& imageMemoryBarriers);
  void copyBufferToImage(std::shared_ptr<Buffer> srcBuffer, std::shared_ptr<Image> dstImage, const std::vector<VkBufferImageCopy>& regions);
  // !TODO void begin(VkCommandBufferUsageFlags flags with VkCommandBufferInheritanceInfo);
  ~Command_Buffer();
private:
  std::shared_ptr<const Device>  m_device;
  std::shared_ptr<Command_Pool>  m_commandPool;
};

}
