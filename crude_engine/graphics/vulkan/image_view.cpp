#include <graphics/vulkan/image_view.hpp>
#include <graphics/vulkan/image.hpp>
#include <graphics/vulkan/device.hpp>

namespace crude_engine
{
  

Image_View::Image_View(Shared_Ptr<const Device>        device,
                       Shared_Ptr<const Image>         image,
                       VkFormat                        format,
                       const Image_Subresource_Range&  subresourceRange,
                       const VkComponentMapping&       components)
  :
  m_device(device),
  m_image(image)
{
  VkImageViewCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  vkCreateInfo.pNext                            = nullptr;
  vkCreateInfo.flags                            = 0u;

  vkCreateInfo.viewType                         = imageToViewType(m_image->getType());
  vkCreateInfo.image                            = CRUDE_OBJECT_HANDLE(m_image);
  vkCreateInfo.components                       = components;
  vkCreateInfo.format                           = format;
  vkCreateInfo.subresourceRange                 = subresourceRange;

  VkResult result = vkCreateImageView(CRUDE_OBJECT_HANDLE(m_device), &vkCreateInfo, &getVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create image view");
}

Image_View::~Image_View()
{
  vkDestroyImageView(CRUDE_OBJECT_HANDLE(m_device), m_handle, &getVkAllocationCallbacks());
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
