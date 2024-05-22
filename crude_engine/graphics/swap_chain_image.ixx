module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.swap_chain_image;

import crude.core.shared_ptr;
import crude.graphics.vulkan.vulkan_object;
import crude.graphics.vulkan.image;

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
  explicit Swap_Chain_Image(core::Shared_Ptr<const Device>  device,
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
