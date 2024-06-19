module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.swap_chain_image;

import crude.core.std_containers_heap;
import crude.graphics.vulkan_object;
import crude.graphics.image_2d;

export namespace crude::graphics
{

class Device;

class Swap_Chain_Image : public Image_2D
{
public:
  explicit Swap_Chain_Image(core::shared_ptr<const Device>  device,
                            VkImage                         handle,
                            VkFormat                        format,
                            VkColorSpaceKHR                 colorSpace,
                            VkExtent2D                      extent)
    :
    m_colorSpace(colorSpace),
    Image_2D(device, handle, format, VkExtent3D{ extent.width, extent.height, 1u })
  {}
  ~Swap_Chain_Image()
  {
    // !TODO
    m_handle = VK_NULL_HANDLE;
  }
private:
  VkColorSpaceKHR  m_colorSpace;
};

}
