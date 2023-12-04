#include "image_view.hpp"
#include "image.hpp"
#include "device.hpp"

namespace crude_engine
{
  
Image_View_Create_Info::Image_View_Create_Info(std::shared_ptr<const Device>   device,
                                               std::shared_ptr<const Image>    image,
                                               VkFormat                        format,
                                               const Image_Subresource_Range&  subresourceRange,
                                               const VkComponentMapping&       components)
  :
  device(device),
  image(image),
  format(format),
  subresourceRange(subresourceRange),
  components(components)
{}

Image_View::Image_View(const Image_View_Create_Info& createInfo)
{
  m_device = createInfo.device;
  m_image = createInfo.image;

  VkImageViewCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  vkCreateInfo.pNext                            = nullptr;
  vkCreateInfo.flags                            = 0u;

  vkCreateInfo.viewType                         = imageToViewType(m_image->getType());
  vkCreateInfo.image                            = CRUDE_VULKAN_01_HANDLE(createInfo.image);
  vkCreateInfo.components                       = createInfo.components;
  vkCreateInfo.format                           = createInfo.format;
  vkCreateInfo.subresourceRange                 = createInfo.subresourceRange;

  VkResult result = vkCreateImageView(CRUDE_VULKAN_01_HANDLE(m_device), &vkCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create image view");
}

Image_View::~Image_View()
{
  vkDestroyImageView(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}
  
VkImageViewType Image_View::imageToViewType(VkImageType imageType)
{
  switch (imageType) 
  {
    case VK_IMAGE_TYPE_1D:  return VK_IMAGE_VIEW_TYPE_1D;
    case VK_IMAGE_TYPE_2D:  return VK_IMAGE_VIEW_TYPE_2D;
    case VK_IMAGE_TYPE_3D:  return VK_IMAGE_VIEW_TYPE_3D;
    // !TODO
  }
  return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
}


}
