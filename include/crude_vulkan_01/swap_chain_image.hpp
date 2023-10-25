#pragma once

#include "image.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

namespace crude_vulkan_01 
{

class Swap_Chain;

struct SwapChainImageCreateInfo : public ImageCreateInfo
{
  explicit SwapChainImageCreateInfo(std::shared_ptr<const Device>  device,
                                    VkImage                        handle,
                                    VkFormat                       format,
                                    VkColorSpaceKHR                colorSpace,
                                    VkExtent2D                     extent,
                                    VkImageUsageFlags              usage);
};

//struct SwapChainImageCreateControlledInfo
//{
//
//};

class Swap_Chain_Image : public Image
{
public:
  explicit Swap_Chain_Image(const SwapChainImageCreateInfo& createInfo);
  ~Swap_Chain_Image();
};

}
