module;

export module crude.scene.vertex;

export import crude.math.fuicont;

export namespace crude::scene
{

#pragma pack(push, 1)
struct Vertex_GPU
{
  math::Float3 position;
  math::Float2 texcoord;
  math::Float3 normal;
  math::Float3 tangent;
  math::Float3 bitangent;
};
#pragma pack(pop)

struct Vertex_CPU
{
  math::Float3 position;
  math::Float2 texcoord;
  math::Float3 normal;
  math::Float3 tangent;
  math::Float3 bitangent;
  explicit operator Vertex_GPU() const noexcept
  {
    Vertex_GPU vertexGPU;
    vertexGPU.position  = position;
    vertexGPU.texcoord  = texcoord;
    vertexGPU.normal    = normal;
    vertexGPU.tangent   = tangent;
    vertexGPU.bitangent = bitangent;
    return vertexGPU;
  }
};

}