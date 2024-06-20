#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

module crude.utils.image_loader;

import crude.core.logger;

namespace crude::utils
{

bool loadImage(const char* inPath, Image_Format inFormat, Image& outImage)
{
  core::int32 width;
  core::int32 height;
  core::int32 channels;
  stbi_uc* pixels = stbi_load("textures/texture.jpg", &width, &height, &channels, inFormat);

  if (pixels == nullptr)
  {
    core::logError(core::Debug::Channel::FileIO, "Failed to load texture image \"%s\"!", inPath);
    return false;
  }

  outImage.width = width;
  outImage.height = height;
  outImage.channels = channels;
  outImage.size = width * height * 4;

  if (!pixels) {
    throw std::runtime_error("failed to load texture image!");
  }

  stbi_image_free(pixels);
}

}