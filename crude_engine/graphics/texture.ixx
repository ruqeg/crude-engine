module;

#include <vulkan/vulkan.h>

export module crude.gfx.texture;

export import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{

class Image;
class Image_View;
class Sampler;

}

export namespace crude::gfx
{

class Texture
{
public:
  explicit Texture(core::shared_ptr<vk::Image_View> imageView, core::shared_ptr<vk::Sampler> sampler);
  void setImageView(core::shared_ptr<vk::Image_View> imageView);
  void setSampler(core::shared_ptr<vk::Sampler> sampler);
  core::shared_ptr<vk::Image_View> getImageView();
  core::shared_ptr<vk::Sampler> getSampler();
  core::shared_ptr<const vk::Image_View> getImageView() const;
  core::shared_ptr<const vk::Sampler> getSampler() const;
private:
  core::shared_ptr<vk::Image_View>  m_imageView;
  core::shared_ptr<vk::Sampler>     m_sampler;
};

}
