#pragma once

#include "image.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

namespace crude_vulkan_01 
{

class Swap_Chain;

struct Swap_Chain_Image_Create_Info : public Image_From_Handle_Create_Info
{
  VkColorSpaceKHR  colorSpace;
  explicit Swap_Chain_Image_Create_Info(std::shared_ptr<const Device>  device,
                                       VkImage                         handle,
                                       VkFormat                        format,
                                       VkColorSpaceKHR                 colorSpace,
                                       VkExtent2D                      extent,
                                       VkImageUsageFlags               usage);
};

//struct SwapChainImageCreateControlledInfo
//{
//
//};

class Swap_Chain_Image : public Image
{
public:
  explicit Swap_Chain_Image(const Swap_Chain_Image_Create_Info& createInfo);
  ~Swap_Chain_Image();
private:
  VkColorSpaceKHR  m_colorSpace;
};

}
