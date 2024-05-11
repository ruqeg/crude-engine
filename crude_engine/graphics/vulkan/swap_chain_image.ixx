module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.swap_chain_image;

import crude_engine.core.shared_ptr;
import crude_engine.graphics.vulkan.vulkan_object;
import crude_engine.graphics.vulkan.image;

export namespace crude_engine 
{

//struct SwapChainImageCreateControlledInfo
//{
//
//};

class Device;

class Swap_Chain_Image : public Image
{
public:
  explicit Swap_Chain_Image(Shared_Ptr<const Device>  device,
                            VkImage                   handle,
                            VkFormat                  format,
                            VkColorSpaceKHR           colorSpace,
                            VkExtent2D                extent,
                            VkImageUsageFlags         usage);
  ~Swap_Chain_Image();
private:
  VkColorSpaceKHR  m_colorSpace;
};

}
