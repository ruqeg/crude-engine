#version 450
#extension GL_EXT_shader_16bit_storage: require
#extension GL_EXT_shader_8bit_storage: require
#extension GL_GOOGLE_include_directive: require

#include "crude/utils.h"
#include "crude/scene.h"

layout(binding=0, row_major) uniform PerFrame
{
  Camera camera;
};

layout(binding=6) uniform sampler2D inAlbedo;
layout(binding=7) uniform sampler2D inMetallicRoughness;
layout(binding=8) uniform sampler2D inNormalMap;

layout (location=0) in PerVertexData
{
  vec2 texCoord;
  vec3 worldSmoothNormal;
  vec3 viewSmoothNormal;
  vec3 viewPosition;
  vec3 worldPosition;
  vec4 color;
};

layout(location=0) out vec4 outAlbedo;
layout(location=1) out vec2 outMetallicRoughness;
layout(location=2) out vec4 outPackedSurfaceNormalTextureNormal;

void main()
{
  const vec4 albedo = texture(inAlbedo, texCoord);
  const vec2 metallicRoughness = texture(inMetallicRoughness, texCoord).xy;
  const vec3 normalMapTexel = texture(inNormalMap, texCoord).xyz;
  
  const vec3 viewSurfaceNormal = normalize(viewSmoothNormal);
  const vec3 textureNormal = perturbNormal(viewSurfaceNormal, viewPosition, texCoord, normalMapTexel);

  const vec2 packedTextureNormal = packOctahedron(normalize((vec4(textureNormal, 0) * camera.viewToWorld).xyz));
  const vec2 packedSurfaceNormal = packOctahedron(normalize(worldSmoothNormal));

  outAlbedo = albedo;
  outMetallicRoughness = metallicRoughness;
  outPackedSurfaceNormalTextureNormal = vec4(packedSurfaceNormal, packedTextureNormal);
}