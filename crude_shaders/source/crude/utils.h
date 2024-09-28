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
// http://www.thetenthplanet.de/archives/1180
mat3 cotangentFrame(vec3 N, vec3 p, vec2 uv)
{
  // get edge vec­tors of the pix­el tri­an­gle
  vec3 dp1  = dFdx(p);
  vec3 dp2  = dFdy(p);
  vec2 duv1 = dFdx(uv);
  vec2 duv2 = dFdy(uv);

  // solve the lin­ear sys­tem
  vec3 dp2perp = cross(dp2, N);
  vec3 dp1perp = cross(N, dp1);
  vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
  vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

  // con­struct a scale-invari­ant frame 
  float invmax = inversesqrt(max(dot(T, T), dot(B, B)));
  return mat3(T * invmax, B * invmax, N);
}

vec3 perturbNormal(vec3 N, vec3 V, vec2 texcoord, vec3 normalMapTex)
{
  // assume N, the interpolated vertex normal and
  // V, the view vector (vertex to eye)
  normalMapTex = normalMapTex * (255.0 / 127.0 - 128.0 / 127.0);
  mat3 TBN = cotangentFrame(N, -V, texcoord);
  return normalize(TBN * normalMapTex);
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
