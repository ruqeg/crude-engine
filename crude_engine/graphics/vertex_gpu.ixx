module;

#include <vulkan/vulkan.h>

export module crude.graphics.vertex_gpu;

export import crude.core.alias;
export import crude.core.std_containers_stack;
export import crude.math.fuicont;
export import crude.scene.vertex_cpu;

export namespace crude::graphics
{

#pragma pack(push, 1)
struct Vertex_GPU
{
  Vertex_GPU() = default;
  Vertex_GPU(const scene::Vertex_CPU& vertexCpu)
    : position(vertexCpu.position)
    , tmpcolor(vertexCpu.tmpcolor)
    , texcoord(vertexCpu.texcoord)
    , normal(vertexCpu.normal)
    , tangent(vertexCpu.tangent)
    , bitangent(vertexCpu.bitangent)
  {}

  math::Float3 position;
  math::Float3 tmpcolor;
  math::Float2 texcoord;
  math::Float3 normal;
  math::Float3 tangent;
  math::Float3 bitangent;

  static const VkVertexInputBindingDescription& getBindingDescription()
  {
    static VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding   = 0;
    bindingDescription.stride    = sizeof(Vertex_GPU);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
  }

  static const core::array<VkVertexInputAttributeDescription, 6>& getAttributeDescriptions()
  {
    static core::array<VkVertexInputAttributeDescription, 6> attributeDescriptions{};
    attributeDescriptions[0].binding  = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset   = offsetof(Vertex_GPU, position);

    attributeDescriptions[1].binding  = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset   = offsetof(Vertex_GPU, tmpcolor);

    attributeDescriptions[2].binding  = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format   = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset   = offsetof(Vertex_GPU, texcoord);

    attributeDescriptions[3].binding  = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[3].offset   = offsetof(Vertex_GPU, normal);

    attributeDescriptions[4].binding  = 0;
    attributeDescriptions[4].location = 4;
    attributeDescriptions[4].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[4].offset   = offsetof(Vertex_GPU, tangent);

    attributeDescriptions[5].binding  = 0;
    attributeDescriptions[5].location = 5;
    attributeDescriptions[5].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[5].offset   = offsetof(Vertex_GPU, bitangent);

    return attributeDescriptions;
  }
};
#pragma pack(pop)

}