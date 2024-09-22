#version 450
#extension GL_GOOGLE_include_directive: require

#include "crude/utils.h"

layout(binding=6) uniform sampler2D inAlbedo;
layout(binding=7) uniform sampler2D inMetallicRoughness;
layout(binding=8) uniform sampler2D inNormalMap;

layout (location=0) in PerVertexData
{
  vec2 texCoord;
  vec3 surfaceNormal;
  vec3 worldPosition;
};

layout(location=0) out vec4 outAlbedo;
layout(location=1) out vec2 outMetallicRoughness;
layout(location=2) out vec4 outPackedSurfaceNormalTextureNormal;

void main()
{
  const vec4 albedo = texture(inAlbedo, texCoord);
  const vec2 metallicRoughness = texture(inMetallicRoughness, texCoord).xy;
  const vec3 normalMapTexel = texture(inNormalMap, texCoord).xyz;

  const vec3 normal = normalize(surfaceNormal);
  const vec3 textureNormal = normalFromNormalMap(normalMapTexel, tbnFromNormalAndUV(worldPosition, normal, texCoord));

  const vec2 packedTextureNormal = packOctahedron(textureNormal);
  const vec2 packedSurfaceNormal = packOctahedron(normal);
  
  outAlbedo = albedo;
  outMetallicRoughness = metallicRoughness;
  outPackedSurfaceNormalTextureNormal = vec4(packedSurfaceNormal, packedTextureNormal);
}
