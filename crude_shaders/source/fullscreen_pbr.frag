#version 450

layout(binding=0) uniform sampler2D albedoSampler;
layout(binding=1) uniform sampler2D depthSampler;

layout (location=0) in PerVertexData
{
  vec2 texCoord;
};

layout(location=0) out vec4 outColor;

void main()
{
  outColor = texture(albedoSampler, texCoord);
}
