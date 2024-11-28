module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.subpass_description;

import crude.core.std_containers_stack;
import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{

class Device;

class Subpass_Description final : public VkSubpassDescription
{
public:
  struct Initialize_Color;
  struct Initialize_Depth;
  struct Initialize_Color_Depth;
  struct Initialize_Color_Array;
  struct Initialize_Color_Array_Depth;
  struct Initialize_Color_Depth_Resolve;
  struct Initialize_Color_Resolve;
public:
  Subpass_Description() noexcept;
  ~Subpass_Description() noexcept;
  Subpass_Description(Subpass_Description& other) noexcept;
  Subpass_Description(Subpass_Description&& other) noexcept;
  Subpass_Description& operator=(Subpass_Description& other) noexcept;
  Subpass_Description& operator=(Subpass_Description&& other) noexcept;
public:
  explicit Subpass_Description(const Initialize_Depth& initialize) noexcept;
  explicit Subpass_Description(const Initialize_Color& initialize) noexcept;
  explicit Subpass_Description(const Initialize_Color_Array& initialize) noexcept;
  explicit Subpass_Description(const Initialize_Color_Resolve& initialize) noexcept;
  explicit Subpass_Description(const Initialize_Color_Depth& initialize) noexcept;
  explicit Subpass_Description(const Initialize_Color_Array_Depth& initialize) noexcept;
  explicit Subpass_Description(const Initialize_Color_Depth_Resolve& initialize) noexcept;
private:
  explicit Subpass_Description(VkPipelineBindPoint pipelineBindPoint) noexcept;
};

struct Subpass_Description::Initialize_Depth
{
  VkImageLayout depthStencilLayout;
};

struct Subpass_Description::Initialize_Color
{
  VkImageLayout colorLayout;
};

struct Subpass_Description::Initialize_Color_Array
{
  core::span<const VkImageLayout> colorLayouts;
};

struct Subpass_Description::Initialize_Color_Depth
{
  VkImageLayout colorLayout;
  VkImageLayout depthStencilLayout;
};

struct Subpass_Description::Initialize_Color_Array_Depth
{
  core::span<const VkImageLayout> colorLayouts; 
  VkImageLayout                   depthStencilLayout;
};

struct Subpass_Description::Initialize_Color_Depth_Resolve
{
  VkImageLayout colorLayout;
  VkImageLayout depthStencilLayout;
  VkImageLayout resolveLayout;
};

struct Subpass_Description::Initialize_Color_Resolve
{
  VkImageLayout colorLayout;
  VkImageLayout resolveLayout;
};

}