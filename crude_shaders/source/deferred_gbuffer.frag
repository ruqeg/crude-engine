#version 450
#extension GL_GOOGLE_include_directive: require

#include "crude/utils.h"

layout(binding=6) uniform sampler2D inAlbedo;
layout(binding=7) uniform sampler2D inMetallicRoughness;
layout(binding=8) uniform sampler2D inNormalMap;

layout (location=0) in PerVertexData
{
  vec2 texCoord;
  vec3 worldTangent;
  vec3 worldBitangent;
  vec3 worldNormal;
};

layout(location=0) out vec4 outAlbedo;
layout(location=1) out vec2 outMetallicRoughness;
layout(location=2) out vec4 outGeometryNormalTextureNormal;

void main()
{
  const vec4 albedo = texture(inAlbedo, texCoord);
  const vec2 metallicRoughness = texture(inMetallicRoughness, texCoord).xy;
  const vec3 normalMapTexel = texture(inNormalMap, texCoord).xyz;

  const vec3 textureNormal = normalFromNormalMap(normalMapTexel, worldTangent, worldBitangent, worldNormal);
  
  const vec2 packedTextureNormal = packOctahedron(textureNormal);
  const vec2 packedGeometryNormal = packOctahedron(worldNormal);

  outAlbedo = albedo;
  outMetallicRoughness = metallicRoughness;
  outGeometryNormalTextureNormal = vec4(packedGeometryNormal, packedTextureNormal);
}
