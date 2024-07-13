module;

#include <cmath>

export module crude.scene.image;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
import crude.core.logger;

export namespace crude::scene
{

enum Image_Format
{
  IMAGE_FORMAT_DEFAULT, // only used for desired_channels
  IMAGE_FORMAT_GREY,
  IMAGE_FORMAT_GREY_ALPHA,
  IMAGE_FORMAT_RGB,
  IMAGE_FORMAT_RGB_ALPHA 
};

class Image
{
public:
  explicit Image(core::shared_ptr<const core::byte[]> texels, Image_Format format, core::int32 width, core::int32 height) noexcept
    : m_texels(texels)
    , m_format(format)
    , m_width(width)
    , m_height(height)
  {}
public:
  core::span<const core::byte> getTexelsSpan() const noexcept { return core::span(getTexels().get(), getSizeBytes()); };
  core::shared_ptr<const core::byte[]> getTexels() const noexcept { return m_texels; };
  core::int64 getSizeBytes() const noexcept { return 4 * m_width * m_height; }
  core::int64 getWidth() const noexcept { return m_width; }
  core::int64 getHeight() const noexcept { return m_height; }
  core::int64 calculateMaximumMipLevelsCount() const noexcept { return std::floor(std::log2(std::max(m_width, m_height))) + 1; }
private:
  core::shared_ptr<const core::byte[]>  m_texels;
  core::int32                           m_width;
  core::int32                           m_height;
  Image_Format                          m_format;
};

}