#ifndef CRUDE_SHADERS_UTILS_H
#define CRUDE_SHADERS_UTILS_H

vec2 nonZeroSign(vec2 v)
{
  return vec2(v.x >= 0.0 ? 1.0 : -1.0, v.y >= 0.0 ? 1.0 : -1.0);
}

vec2 packOctahedron(vec3 v)
{
  vec2 p = v.xy / (abs(v.x) + abs(v.y) + abs(v.z));
  return (v.z <= 0.0) ? ((vec2(1.0, 1.0) - abs(p.yx)) * nonZeroSign(p)) : p;
}

vec3 unpackOctahedron(vec2 oct)
{
  vec3 v = vec3(oct, 1.0 - abs(oct.x) - abs(oct.y));
  if (v.z < 0)
    v.xy = (vec2(1.0, 1.0) - abs(v.yx)) * nonZeroSign(v.xy);
  return normalize(v);
}

// Per-Pixel Tangent Space Normal Mapping
// http://hacksoflife.blogspot.ch/2009/11/per-pixel-tangent-space-normal-mapping.html
mat3 tbnFromNormalAndUV(vec3 worldPosition, vec3 surfaceNormal, vec2 uv)
{
  const vec3 q0 = dFdx(worldPosition.xyz);
  const vec3 q1 = dFdy(worldPosition.xyz);
  const vec2 st0 = dFdx(uv.st);
  const vec2 st1 = dFdy(uv.st);

  const float scale = sign(st1.t * st0.s - st0.t * st1.s);

  const vec3 tangent = normalize((q0 * st1.t - q1 * st0.t) * scale);
  const vec3 bitangent = normalize((-q0 * st1.s + q1 * st0.s) * scale);

  const mat3 tbn = mat3(tangent, bitangent, surfaceNormal);
  return tbn;
}

vec3 normalFromNormalMap(vec3 normalMapTex, mat3 tbn)
{
  normalMapTex.xy *= (float(gl_FrontFacing) * 2.0 - 1.0);
  const vec3 textureNormal = normalize(tbn * normalMapTex);
  return textureNormal;
}

vec3 normalFromNormalMap(vec3 normalMapTex, vec3 geometryTangent, vec3 geometryBitangent, vec3 geometryNormal)
{
  return normalFromNormalMap(normalMapTex, mat3(geometryTangent, geometryBitangent, geometryNormal));
}

vec4 viewFromDepth(vec2 texCoord, float depth, mat4x4 clipToView)
{
  const float z = depth;
  const float x = texCoord.x * 2.f - 1.f;
  const float y = texCoord.y * 2.f - 1.f;
  const vec4 projPosition = vec4(x, y, z, 1.f);
  const vec4 positonVS = projPosition * clipToView;
  const vec4 viewPosition = positonVS / positonVS.w;
  return viewPosition;
}

#endif // CRUDE_SHADERS_UTILS_H
