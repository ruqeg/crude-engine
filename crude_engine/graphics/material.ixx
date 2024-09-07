module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.material;

export import crude.core.std_containers_heap;

export namespace crude::graphics
{

class Texture;

struct Material
{
  core::shared_ptr<Texture> albedo;
  core::shared_ptr<Texture> normal;
  core::shared_ptr<Texture> metallicRoughness;
};

}
