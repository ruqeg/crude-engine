#version 450

layout(binding=0) uniform sampler2D inAlbedo;
layout(binding=1) uniform sampler2D inMetallicRoughness;
layout(binding=2) uniform sampler2D inGeometryNormalTextureNormal;
layout(binding=3) uniform sampler2D inDepth;

layout (location=0) in PerVertexData
{
  vec2 texCoord;
};

layout(location=0) out vec4 outColor;

void main()
{
  outColor = texture(inAlbedo, texCoord);
}
