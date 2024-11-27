module crude.gfx.texture;

import crude.gfx.vk.image_view;
import crude.gfx.vk.sampler;

namespace crude::gfx
{

Texture::Texture(core::shared_ptr<vk::Image_View> imageView, core::shared_ptr<vk::Sampler> sampler)
  : m_imageView(imageView), m_sampler(sampler) {}
void Texture::setImageView(core::shared_ptr<vk::Image_View> imageView) { m_imageView = imageView; }
void Texture::setSampler(core::shared_ptr<vk::Sampler> sampler) { m_sampler = sampler; }
core::shared_ptr<vk::Image_View> Texture::getImageView() { return m_imageView; }
core::shared_ptr<vk::Sampler> Texture::getSampler() { return m_sampler; }
core::shared_ptr<const vk::Image_View> Texture::getImageView() const { return m_imageView; }
core::shared_ptr<const vk::Sampler> Texture::getSampler() const { return m_sampler; }

}
