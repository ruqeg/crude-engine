#version 450

layout(binding=6) uniform sampler2D albedo;
layout(binding=7) uniform sampler2D metallicRoughness;
layout(binding=8) uniform sampler2D normal;

layout (location=0) in PerVertexData
{
  vec3 color;
  vec2 texCoord;
};

layout(location=0) out vec4 outColor;

void main()
{
  outColor = texture(albedo, texCoord);
}
