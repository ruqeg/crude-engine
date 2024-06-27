module;

export module crude.scene.vertex_cpu;

export import crude.core.std_containers_stack;
export import crude.math.fuicont;

export namespace crude::scene
{

struct Vertex_CPU
{
  math::Float3 position;
  math::Float3 tmpcolor;
  math::Float2 texcoord;
  math::Float3 normal;
  math::Float3 tangent;
  math::Float3 bitangent;
};

}