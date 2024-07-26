module crude.graphics.texture;

import crude.graphics.image_view;
import crude.graphics.sampler;

namespace crude::graphics
{

Texture::Texture(core::shared_ptr<Image_View> imageView, core::shared_ptr<Sampler> sampler)
  : m_imageView(imageView), m_sampler(sampler) {}
void Texture::setImageView(core::shared_ptr<Image_View> imageView) { m_imageView = imageView; }
void Texture::setSampler(core::shared_ptr<Sampler> sampler) { m_sampler = sampler; }
core::shared_ptr<Image_View> Texture::getImageView() { return m_imageView; }
core::shared_ptr<Sampler> Texture::getSampler() { return m_sampler; }
core::shared_ptr<const Image_View> Texture::getImageView() const { return m_imageView; }
core::shared_ptr<const Sampler> Texture::getSampler() const { return m_sampler; }

}
