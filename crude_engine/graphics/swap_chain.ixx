module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.swap_chain;

import crude.core.shared_ptr;
import crude.core.optional;
import crude.core.std_containers_stack;
import crude.core.std_containers_heap;
import crude.graphics.vulkan.vulkan_object;

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
  core::Optional<core::uint32> getImageIndex() const;
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
  explicit Swap_Chain(core::Shared_Ptr<const Device>   device,
                      core::Shared_Ptr<const Surface>  surface,
                      const VkSurfaceFormatKHR&        surfaceFormat,
                      const VkExtent2D&                extent,
                      VkImageUsageFlags                imageUsage,
                      core::uint32                     minImageCount,
                      core::uint32                     imageArrayLayers,
                      VkSharingMode                    imageSharingMode,
                      const core::span<core::uint32>&  queueFamilyIndices,
                      VkSurfaceTransformFlagBitsKHR    preTransform,
                      VkCompositeAlphaFlagBitsKHR      compositeAlpha,
                      VkPresentModeKHR                 presentMode,
                      VkBool32                         clipped,
                      VkSwapchainCreateFlagsKHR        flags,
                      core::Shared_Ptr<Swap_Chain>     oldSwapchain);
  ~Swap_Chain();
  const core::vector<core::Shared_Ptr<Swap_Chain_Image>>& getSwapchainImages();
  Swap_Chain_Next_Image acquireNextImage(const core::Optional<core::Shared_Ptr<Semaphore>>&  semaphore = core::nullopt,
                                         const core::Optional<core::Shared_Ptr<Fence>>&      fence = core::nullopt,
                                         core::uint64                                        timeout = UINT64_MAX);
  const VkImageUsageFlags getImageUsage() const;
  const VkSurfaceFormatKHR getSurfaceFormat() const;
  const VkExtent2D getExtent() const;

private:
  core::Shared_Ptr<const Device>                   m_device;
  core::Shared_Ptr<const Surface>                  m_surface;
  core::vector<core::Shared_Ptr<Swap_Chain_Image>> m_swapChainImages;

  const VkImageUsageFlags  m_imageUsage;
  const VkSurfaceFormatKHR m_surfaceFormat;
  const VkExtent2D         m_extent;
};

}
