module;

#include <stb_image.h>

export module crude.resources.image_loader;

export import crude.core.optional;
export import crude.scene.image;
import crude.core.logger;

namespace crude::resources
{

core::int32 imageFormatToStbiFormat(scene::Image_Format format)
{
  switch (format)
  {
  case scene::IMAGE_FORMAT_DEFAULT: return STBI_default;
  case scene::IMAGE_FORMAT_GREY: return STBI_grey;
  case scene::IMAGE_FORMAT_GREY_ALPHA: return STBI_grey_alpha;
  case scene::IMAGE_FORMAT_RGB: return STBI_rgb;
  case scene::IMAGE_FORMAT_RGB_ALPHA: return STBI_rgb_alpha;
  default:
    core::logError(core::Debug::Channel::FileIO, "Unknow type for imageFormatToStbiFormat()");
  }
  return STBI_default;
}

}

export namespace crude::resources
{

core::Optional<scene::Image> loadImage(const char* path, scene::Image_Format format) noexcept
{
  static_assert(sizeof(stbi_uc) == sizeof(core::byte));

  core::int32 width, height, channels;
  stbi_uc* texels = stbi_load(path, &width, &height, &channels, imageFormatToStbiFormat(format));
  if (texels == nullptr)
  {
    core::logError(core::Debug::Channel::FileIO, "Failed to load texture image \"%s\"!", path);
    return core::nullopt;
  }
  
  core::shared_ptr<core::byte[]> texelsShared = core::shared_ptr<core::byte[]>(texels, [](core::byte* texels) {
    stbi_image_free(texels);
  });
  return scene::Image(texelsShared, format, width, height);
}

}