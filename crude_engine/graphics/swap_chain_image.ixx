module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.swap_chain_image;

import crude.core.std_containers_heap;
import crude.graphics.vulkan_object;
import crude.graphics.image;

export namespace crude::graphics
{

//struct SwapChainImageCreateControlledInfo
//{
//
//};

class Device;

class Swap_Chain_Image : public Image
{
public:
  explicit Swap_Chain_Image(core::shared_ptr<const Device>  device,
                            VkImage                         handle,
                            VkFormat                        format,
                            VkColorSpaceKHR                 colorSpace,
                            VkExtent2D                      extent,
                            VkImageUsageFlags               usage);
  ~Swap_Chain_Image();
private:
  VkColorSpaceKHR  m_colorSpace;
};

}
