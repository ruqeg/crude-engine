#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <vector>
#include <optional>
#include <limits>

namespace crude_vulkan_01
{

class Surface;
class Device;
class Swap_Chain;
class Swap_Chain_Image;
class Fence;
class Semaphore;

struct Swap_Chain_Create_Info
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

  explicit Swap_Chain_Create_Info(std::shared_ptr<const Device>   device,
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

class Swap_Chain_Next_Image
{
public:
  std::optional<uint32> getImageIndex() const;
  VkResult getResult() const;
  bool outOfDate() const;
  bool failedAcquire() const;
private:
  Swap_Chain_Next_Image(uint32 imageIndex, VkResult result);
private:
  uint32    m_imageIndex;
  VkResult  m_result;

  friend class Swap_Chain;
};

class Swap_Chain : public TObject<VkSwapchainKHR>
{
public:
  explicit Swap_Chain(const Swap_Chain_Create_Info& createInfo);
  ~Swap_Chain();
  const std::vector<std::shared_ptr<Swap_Chain_Image>>& getSwapchainImages();
  Swap_Chain_Next_Image acquireNextImage(const std::optional<std::shared_ptr<Semaphore>>&  semaphore = std::nullopt,
                                         const std::optional<std::shared_ptr<Fence>>&      fence = std::nullopt, 
                                         uint64                                            timeout = std::numeric_limits<uint64>::max());
  const VkImageUsageFlags getImageUsage() const;
  const VkSurfaceFormatKHR getSurfaceFormat() const;
  const VkExtent2D getExtent() const;
private:
  std::shared_ptr<const Device> m_device;
  std::shared_ptr<const Surface> m_surface;
  std::vector<std::shared_ptr<Swap_Chain_Image>> m_swapChainImages;

  const VkImageUsageFlags m_imageUsage;
  const VkSurfaceFormatKHR m_surfaceFormat;
  const VkExtent2D m_extent;
};

}
