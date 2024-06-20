module;

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

export module crude.utils.image_loader;

export import crude.core.std_containers_heap;
export import crude.core.alias;

export namespace crude::utils
{

struct Image
{
  core::vector<core::byte>  pixels;
  core::int64               size;
  core::int32               width;
  core::int32               height;
  core::int32               channels;
};

enum Image_Format
{
  IMAGE_FORMAT_DEFAULT     = STBI_default, // only used for desired_channels
  IMAGE_FORMAT_GREY        = STBI_grey,
  IMAGE_FORMAT_GREY_ALPHA  = STBI_grey_alpha,
  IMAGE_FORMAT_RGB         = STBI_rgb,
  IMAGE_FORMAT_RGB_ALPHA   = STBI_rgb_alpha
};

bool loadImage(const char* inPath, Image_Format inFormat, Image& outImage);

}