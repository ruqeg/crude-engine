module;

#include <vulkan/vulkan.hpp>

export module crude.scene.vertex;

export import crude.core.std_containers_stack;
export import crude.math.fuicont;

export namespace crude::scene
{

#pragma pack(push, 1)
struct Vertex_GPU
{
  math::Float3 position;
  math::Float3 tmpcolor;
  math::Float2 texcoord;
  math::Float3 normal;
  math::Float3 tangent;
  math::Float3 bitangent;

  // !TODO MOVE :D
  static const VkVertexInputBindingDescription& getBindingDescription()
  {
    static VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex_GPU);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
  }

  static const core::array<VkVertexInputAttributeDescription, 6>& getAttributeDescriptions()
  {
    static core::array<VkVertexInputAttributeDescription, 6> attributeDescriptions{};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex_GPU, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex_GPU, tmpcolor);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex_GPU, texcoord);

    attributeDescriptions[3].binding = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[3].offset = offsetof(Vertex_GPU, normal);

    attributeDescriptions[4].binding = 0;
    attributeDescriptions[4].location = 4;
    attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[4].offset = offsetof(Vertex_GPU, tangent);

    attributeDescriptions[5].binding = 0;
    attributeDescriptions[5].location = 5;
    attributeDescriptions[5].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[5].offset = offsetof(Vertex_GPU, bitangent);

    return attributeDescriptions;
  }
};
#pragma pack(pop)

struct Vertex_CPU
{
  math::Float3 position;
  math::Float3 tmpcolor;
  math::Float2 texcoord;
  math::Float3 normal;
  math::Float3 tangent;
  math::Float3 bitangent;
  explicit operator Vertex_GPU() const noexcept
  {
    Vertex_GPU vertexGPU;
    vertexGPU.position  = position;
    vertexGPU.tmpcolor = tmpcolor;
    vertexGPU.texcoord  = texcoord;
    vertexGPU.normal    = normal;
    vertexGPU.tangent   = tangent;
    vertexGPU.bitangent = bitangent;
    return vertexGPU;
  }
};

}