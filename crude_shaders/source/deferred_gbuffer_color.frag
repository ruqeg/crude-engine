#version 450

layout(binding=6) uniform sampler2D inAlbedo;

layout (location=0) in PerVertexData
{
  vec2 texCoord;
};

layout(location=0) out vec4 outAlbedo;

void main()
{
  const vec4 albedo = texture(inAlbedo, texCoord);
  outAlbedo = albedo;
}
