module;

#include <cmath>

export module crude.scene.image;

export import crude.core.alias;

export namespace crude::scene
{

core::int64 calculateMaximumMipLevelsCount(core::int32 width, core::int32 height) noexcept
{
  return std::floor(std::log2(std::max(width, height))) + 1;
}

}