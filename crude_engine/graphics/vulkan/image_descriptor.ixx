module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.image_descriptor;

export import crude.gfx.vk.descriptor_set_layout_binding;
export import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{

class Image_View;
class Sampler;

class Image_Descriptor : public Descriptor_Set_Layout_Binding
{
public:
  bool update(core::shared_ptr<const Image_View> imageView, core::shared_ptr<const Sampler> sampler) noexcept;
  void write(VkWriteDescriptorSet& writeDescriptorSet);
protected:
  explicit Image_Descriptor(VkDescriptorType descriptorType, core::uint32 binding, VkShaderStageFlags stageFlags) noexcept;
protected:
  VkDescriptorImageInfo m_descriptorImageInfo;
};

class Combined_Image_Sampler_Descriptor : public Image_Descriptor
{
public:
  explicit Combined_Image_Sampler_Descriptor(core::uint32 binding, VkShaderStageFlags stageFlags) noexcept;
};

}