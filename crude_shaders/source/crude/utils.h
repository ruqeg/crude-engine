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

vec3 normalFromNormalMap(vec3 normalMapTex, 
                         vec3 geomtryTangent, 
                         vec3 geomtryBitangent,
                         vec3 geomtryNormal)
{
  const vec3 fNormalTex = normalMapTex * 2.0 - 1.0;
  const mat3x3 tbn = mat3x3(geomtryTangent, geomtryBitangent, geomtryNormal);
  const vec3 normal = normalize(fNormalTex * tbn);
  return normal;
}

#endif // CRUDE_SHADERS_UTILS_H
