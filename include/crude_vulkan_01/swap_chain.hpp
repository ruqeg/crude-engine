#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <memory>
#include <vector>

namespace crude_vulkan_01
{

class Surface;
class Device;
class Swap_Chain;

struct SwapChainCreateInfo
{
  std::shared_ptr<const Device>   device;
  std::shared_ptr<const Surface>  surface;
  VkSurfaceFormatKHR              surfaceFormat;
  VkExtent2D                      extent;
  VkImageUsageFlags               imageUsage;
  uint32_t                        minImageCount;
  uint32_t                        imageArrayLayers;
  VkSharingMode                   imageSharingMode;
  std::vector<uint32>             queueFamilyIndices;
  VkSurfaceTransformFlagBitsKHR   preTransform;
  VkCompositeAlphaFlagBitsKHR     compositeAlpha;
  VkPresentModeKHR                presentMode;
  VkBool32                        clipped;
  VkSwapchainCreateFlagsKHR       flags;
  std::shared_ptr<Swap_Chain>     oldSwapchain;

  explicit SwapChainCreateInfo(std::shared_ptr<const Device>   device,
                               std::shared_ptr<const Surface>  surface,
                               const VkSurfaceFormatKHR&       surfaceFormat,
                               const VkExtent2D&               extent,
                               VkImageUsageFlags               imageUsage,
                               uint32_t                        minImageCount,
                               uint32_t                        imageArrayLayers,
                               VkSharingMode                   imageSharingMode,
                               const std::vector<uint32>&      queueFamilyIndices,
                               VkSurfaceTransformFlagBitsKHR   preTransform,
                               VkCompositeAlphaFlagBitsKHR     compositeAlpha,
                               VkPresentModeKHR                presentMode,
                               VkBool32                        clipped,
                               VkSwapchainCreateFlagsKHR       flags,
                               std::shared_ptr<Swap_Chain>     oldSwapchain);
};

class Swap_Chain : public TObject<VkSwapchainKHR>
{
public:
  explicit Swap_Chain(const SwapChainCreateInfo& createInfo);
  ~Swap_Chain();
private:
  std::shared_ptr<const Device>   m_device;
  std::shared_ptr<const Surface>  m_surface;
};

}
