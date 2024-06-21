module;

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

export module crude.scene.image;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
import crude.core.logger;

export namespace crude::scene
{

enum Image_Format
{
  IMAGE_FORMAT_DEFAULT    = STBI_default, // only used for desired_channels
  IMAGE_FORMAT_GREY       = STBI_grey,
  IMAGE_FORMAT_GREY_ALPHA = STBI_grey_alpha,
  IMAGE_FORMAT_RGB        = STBI_rgb,
  IMAGE_FORMAT_RGB_ALPHA  = STBI_rgb_alpha
};

class Image
{
public:
  bool load(const char* path, Image_Format format) noexcept
  {
    stbi_uc* texels = stbi_load(path, &m_width, &m_height, &m_channels, format);
    if (texels == nullptr)
    {
      core::logError(core::Debug::Channel::FileIO, "Failed to load texture image \"%s\"!", path);
      return false;
    }
    m_texels = core::shared_ptr<core::byte>(texels, [](core::byte* texels) {
      stbi_image_free(texels);
    });
    return true;
  }
public:
  core::span<const core::byte> getTexelsSpan() const noexcept { return core::span(getTexels().get(), getSizeBytes()); };
  core::shared_ptr<const core::byte> getTexels() const noexcept { return m_texels; };
  core::int64 getSizeBytes() const noexcept { return 4 * m_width * m_height; }
  core::int64 getWidth() const noexcept { return m_width; }
  core::int64 getHeight() const noexcept { return m_height; }
  core::int64 getChannel() const noexcept { return m_channels; }
private:
  core::shared_ptr<core::byte>  m_texels;
  core::int32                   m_width;
  core::int32                   m_height;
  core::int32                   m_channels;
private:
  static_assert(sizeof(stbi_uc) == sizeof(core::byte));
};

}