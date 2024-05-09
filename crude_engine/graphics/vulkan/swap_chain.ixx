module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.swap_chain;

import crude_engine.core.shared_ptr;
import crude_engine.core.optional;
import crude_engine.core.std_containers_stack;
import crude_engine.core.std_containers_heap;
import crude_engine.graphics.vulkan.vulkan_object;

export namespace crude_engine
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
  Optional<uint32> getImageIndex() const;
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

class Swap_Chain : public Vulkan_Object<VkSwapchainKHR>
{
public:
  explicit Swap_Chain(Shared_Ptr<const Device>       device,
                      Shared_Ptr<const Surface>      surface,
                      const VkSurfaceFormatKHR&      surfaceFormat,
                      const VkExtent2D&              extent,
                      VkImageUsageFlags              imageUsage,
                      uint32_t                       minImageCount,
                      uint32_t                       imageArrayLayers,
                      VkSharingMode                  imageSharingMode,
                      const span<uint32>&            queueFamilyIndices,
                      VkSurfaceTransformFlagBitsKHR  preTransform,
                      VkCompositeAlphaFlagBitsKHR    compositeAlpha,
                      VkPresentModeKHR               presentMode,
                      VkBool32                       clipped,
                      VkSwapchainCreateFlagsKHR      flags,
                      Shared_Ptr<Swap_Chain>         oldSwapchain);
  ~Swap_Chain();
  const vector<Shared_Ptr<Swap_Chain_Image>>& getSwapchainImages();
  Swap_Chain_Next_Image acquireNextImage(const Optional<Shared_Ptr<Semaphore>>&  semaphore = nullopt,
                                         const Optional<Shared_Ptr<Fence>>&      fence = nullopt,
                                         uint64                                  timeout = UINT64_MAX);
  const VkImageUsageFlags getImageUsage() const;
  const VkSurfaceFormatKHR getSurfaceFormat() const;
  const VkExtent2D getExtent() const;

private:
  Shared_Ptr<const Device>             m_device;
  Shared_Ptr<const Surface>            m_surface;
  vector<Shared_Ptr<Swap_Chain_Image>> m_swapChainImages;

  const VkImageUsageFlags  m_imageUsage;
  const VkSurfaceFormatKHR m_surfaceFormat;
  const VkExtent2D         m_extent;
};

}
