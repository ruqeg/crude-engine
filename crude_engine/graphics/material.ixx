module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.material;

export import crude.core.std_containers_heap;

export namespace crude::gfx
{

class Texture;

struct Material
{
  core::shared_ptr<Texture> albedo;
  core::shared_ptr<Texture> normal;
  core::shared_ptr<Texture> metallicRoughness;
  bool isValid() { return albedo && normal && metallicRoughness; }
};

}
