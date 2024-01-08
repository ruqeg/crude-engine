#pragma once

#include <graphics/vulkan/image.hpp>

namespace crude_engine 
{

class Swap_Chain;

//struct SwapChainImageCreateControlledInfo
//{
//
//};

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
