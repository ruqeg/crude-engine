module;

#include <vulkan/vulkan.h>

export module crude.graphics.texture;

export import crude.core.std_containers_heap;

export namespace crude::graphics
{

class Image;
class Sampler;

class Texture
{
public:
  explicit Texture(core::shared_ptr<Image> image, core::shared_ptr<Sampler> sampler);
  void setImage(core::shared_ptr<Image> image);
  void setSampler(core::shared_ptr<Sampler> sampler);
  core::shared_ptr<Image> getImage();
  core::shared_ptr<Sampler> getSampler();
  core::shared_ptr<const Image> getImage() const;
  core::shared_ptr<const Sampler> getSampler() const;
private:
  core::shared_ptr<Image>    m_image;
  core::shared_ptr<Sampler>  m_sampler;
};

}
