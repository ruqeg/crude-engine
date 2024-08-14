module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.swap_chain;

import crude.core.std_containers_heap;
import crude.core.std_containers_stack;
import crude.core.std_containers_heap;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Surface;
class Device;
class Swap_Chain;
class Swap_Chain_Image;
class Fence;
class Semaphore;

class Swap_Chain_Next_Image
{
public:
  core::optional<core::uint32> getImageIndex() const;
  VkResult getResult() const;
  bool outOfDate() const;
  bool failedAcquire() const;
private:
  Swap_Chain_Next_Image(core::uint32 imageIndex, VkResult result);
private:
  core::uint32  m_imageIndex;
  VkResult      m_result;

  friend class Swap_Chain;
};

class Swap_Chain : public Vulkan_Object<VkSwapchainKHR>
{
public:
  explicit Swap_Chain(core::shared_ptr<const Device>   device,
                      core::shared_ptr<const Surface>  surface,
                      const VkSurfaceFormatKHR&        surfaceFormat,
                      const VkExtent2D&                extent,
                      VkImageUsageFlags                imageUsage,
                      core::uint32                     minImageCount,
                      core::uint32                     imageArrayLayers,
                      core::span<const core::uint32>   queueFamilyIndices,
                      VkSurfaceTransformFlagBitsKHR    preTransform,
                      VkCompositeAlphaFlagBitsKHR      compositeAlpha,
                      VkPresentModeKHR                 presentMode,
                      VkBool32                         clipped,
                      VkSwapchainCreateFlagsKHR        flags,
                      core::shared_ptr<Swap_Chain>     oldSwapchain);
  explicit Swap_Chain(core::shared_ptr<const Device>   device,
                      core::shared_ptr<const Surface>  surface,
                      const VkSurfaceFormatKHR&        surfaceFormat,
                      const VkExtent2D&                extent,
                      VkImageUsageFlags                imageUsage,
                      core::uint32                     minImageCount,
                      core::uint32                     imageArrayLayers,
                      VkSurfaceTransformFlagBitsKHR    preTransform,
                      VkCompositeAlphaFlagBitsKHR      compositeAlpha,
                      VkPresentModeKHR                 presentMode,
                      VkBool32                         clipped,
                      VkSwapchainCreateFlagsKHR        flags,
                      core::shared_ptr<Swap_Chain>     oldSwapchain);
  ~Swap_Chain();
public:
  const core::vector<core::shared_ptr<Swap_Chain_Image>>& getSwapchainImages();
  Swap_Chain_Next_Image acquireNextImage(const core::optional<core::shared_ptr<Semaphore>>&  semaphore = core::nullopt,
                                         const core::optional<core::shared_ptr<Fence>>&      fence = core::nullopt,
                                         core::uint64                                        timeout = UINT64_MAX);
  const VkImageUsageFlags getImageUsage() const;
  const VkSurfaceFormatKHR getSurfaceFormat() const;
  const VkExtent2D getExtent() const;
  core::shared_ptr<const Device> getDevice();
private:
  core::shared_ptr<const Device>                   m_device;
  core::shared_ptr<const Surface>                  m_surface;
  core::vector<core::shared_ptr<Swap_Chain_Image>> m_swapChainImages;

  const VkImageUsageFlags  m_imageUsage;
  const VkSurfaceFormatKHR m_surfaceFormat;
  const VkExtent2D         m_extent;
};

}
