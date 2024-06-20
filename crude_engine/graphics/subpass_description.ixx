module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.subpass_description;

import crude.core.optional;
import crude.core.std_containers_stack;
import crude.core.std_containers_heap;

export namespace crude::graphics
{

class Device;

class Subpass_Description final : public VkSubpassDescription
{
public:
  Subpass_Description() noexcept;
  ~Subpass_Description() noexcept;
  Subpass_Description(Subpass_Description& other) noexcept;
  Subpass_Description(Subpass_Description&& other) noexcept;
  Subpass_Description& operator=(Subpass_Description& other) noexcept;
  Subpass_Description& operator=(Subpass_Description&& other) noexcept;
public:
  explicit Subpass_Description(VkImageLayout colorLayout, VkImageLayout depthStencilLayout) noexcept;
private:
  explicit Subpass_Description(VkPipelineBindPoint pipelineBindPoint) noexcept;
};

}