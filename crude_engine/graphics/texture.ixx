module;

#include <vulkan/vulkan.h>

export module crude.graphics.texture;

export import crude.core.std_containers_heap;

export namespace crude::graphics
{

class Image;
class Image_View;
class Sampler;

class Texture
{
public:
  explicit Texture(core::shared_ptr<Image_View> imageView, core::shared_ptr<Sampler> sampler);
  void setImageView(core::shared_ptr<Image_View> imageView);
  void setSampler(core::shared_ptr<Sampler> sampler);
  core::shared_ptr<Image_View> getImageView();
  core::shared_ptr<Sampler> getSampler();
  core::shared_ptr<const Image_View> getImageView() const;
  core::shared_ptr<const Sampler> getSampler() const;
private:
  core::shared_ptr<Image_View>  m_imageView;
  core::shared_ptr<Sampler>     m_sampler;
};

}
