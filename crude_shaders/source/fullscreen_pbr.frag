#version 450

#extension GL_GOOGLE_include_directive: require
#extension GL_EXT_shader_16bit_storage: require
#extension GL_EXT_shader_8bit_storage: require

#include "crude/debug.h"
layout(binding=6) uniform DebugBuffer
{
  PBRDebug pbrDebug;
};

#include "crude/config.h"
#include "crude/scene.h"
#include "crude/utils.h"
#include "crude/pbr.h"

layout(binding=0, row_major) uniform PerFrameBuffer
{
  Camera camera;
};
layout(binding=1) uniform sampler2D inAlbedo;
layout(binding=2) uniform sampler2D inMetallicRoughness;
layout(binding=3) uniform sampler2D inPackedSurfaceNormalTextureNormal;
layout(binding=4) uniform sampler2D inDepth;
layout(binding=5) readonly buffer PointLightsBuffer
{
  PointLight pointLights[];
};

layout (location=0) in PerVertexData
{
  vec2 screenUV;
};

layout(location=0) out vec4 outColor;

vec3 pbrSceneLightOut(vec3 worldPosition, vec3 normal, vec3 albedo, float metalness, float roughness)
{
  const vec3 vertexToCameraDirection = normalize(camera.position - worldPosition);
  const float ndotv = max(dot(normal, vertexToCameraDirection), 0.f);
  
  const vec3 f0 = mix(vec3(0.04, 0.04, 0.04), albedo, metalness);
  
  vec3 eDirect = vec3 (0.0, 0.0, 0.0);
  
  for (uint i = 0; i < pointLights.length(); ++i)
  {
    const vec3  lightIntensity    = pointLights[i].intensity;
    const float lightSourceRadius = pointLights[i].sourceRadius;
    const vec3  lightPosition     = pointLights[i].position;
    
    const vec3  vertexToLight          = lightPosition - worldPosition;
    const float vertexToLightDistance  = length(vertexToLight);
    const vec3  vertexToLightDirection = vertexToLight / vertexToLightDistance;
    
    const vec3 halfVector = normalize((vertexToCameraDirection + vertexToLightDirection) * 0.5);
    
    const float solidAngle = sphereSolidAngle(lightPosition, lightSourceRadius * lightSourceRadius, vertexToCameraDirection);
    
    eDirect += debugCookTorranceBRDF(lightIntensity, solidAngle, albedo, metalness, roughness, f0, normal, vertexToCameraDirection, vertexToLightDirection, halfVector);
  }
  
  const vec3 eDiff = vec3(0.0, 0.0, 0.0);
  const vec3 eSpec = vec3(0.0, 0.0, 0.0);
  const vec2 kRG = vec2(0.0, 0.0);
  
  vec3 lightOut = vec3(0.0, 0.0, 0.0);
  lightOut += eDirect;
  lightOut += eDiff * albedo * (1.0 - metalness);
  lightOut += eSpec * (f0 * kRG.x + kRG.y);
  
  return lightOut;
}

void main()
{
  const float depth                            = texture(inDepth, screenUV).x;
  const vec4  albedo                           = texture(inAlbedo, screenUV);
  const vec2  metallicRoughness                = texture(inMetallicRoughness, screenUV).xy;
  const vec4  packedSurfaceNormalTextureNormal = texture(inPackedSurfaceNormalTextureNormal, screenUV);
  
  const float metalness = metallicRoughness.x;
  const float roughness = metallicRoughness.y;
  
  const vec3 surfaceNormal = unpackOctahedron(packedSurfaceNormalTextureNormal.xy);
  const vec3 textureNormal  = unpackOctahedron(packedSurfaceNormalTextureNormal.zw);
  
  const vec4 viewPosition  = viewFromDepth(screenUV, depth, camera.clipToView);
  const vec4 worldPosition = viewPosition * camera.viewToWorld;
  
  const vec3 lightOut = pbrSceneLightOut(worldPosition.xyz, textureNormal, albedo.rgb, metalness, roughness);
  outColor = vec4(textureNormal, 1);
}