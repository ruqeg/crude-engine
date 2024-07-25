module crude.graphics.texture;

import crude.graphics.image;
import crude.graphics.sampler;

namespace crude::graphics
{

void Texture::setImage(core::shared_ptr<Image> image) { m_image = image; }
void Texture::setSampler(core::shared_ptr<Sampler> sampler) { m_sampler = sampler; }
core::shared_ptr<Image> Texture::getImage() { return m_image; }
core::shared_ptr<Sampler> Texture::getSampler() { return m_sampler; }
core::shared_ptr<const Image> Texture::getImage() const { return m_image; }
core::shared_ptr<const Sampler> Texture::getSampler() const { return m_sampler; }

}
