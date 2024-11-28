module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.swap_chain_image;

import crude.core.std_containers_heap;
import crude.gfx.vk.vulkan_object;
import crude.gfx.vk.image_2d;

export namespace crude::gfx::vk
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
    Image_2D(device, handle, format, extent)
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
