module;

export module crude.scene.vertex_cpu;

export import crude.core.std_containers_stack;
export import crude.math.fuicont;

export namespace crude::scene
{

struct Vertex_CPU
{
  math::Float3A position;
  math::Float3A tmpcolor;
  math::Float2A texcoord;
  math::Float3A normal;
  math::Float3A tangent;
  math::Float3A bitangent;
};

}