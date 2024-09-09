#ifndef CRUDE_SHADERS_PBR_H
#define CRUDE_SHADERS_PBR_H

#include "math.h"

float sphereSolidAngle(vec3 p, float r2, vec3 v, out float d)
{
  d = length(p - v);
  const float d2 = d * d;
  const float h = 1.0 - sqrt(max(1.0 - r2 / d2, 0.0));
  const float w = 2.0 * PI * h;
  return w;
}

float sphereSolidAngle(vec3 lightPosition, float lightRadius2, vec3 vertex)
{
  float d;
  return sphereSolidAngle(lightPosition, lightRadius2, vertex, d);
}

vec3 FNone(vec3 f0)
{
  return f0;
}

vec3 FSchlick(vec3 f0, float vdoth)
{
  return f0 + (1.0 - f0) * pow((1.0 - vdoth), 5.0);
}

vec3 FCookTorrance(vec3 f0, float vdoth)
{
  const vec3 sqrtf0 = sqrt(f0);
  const vec3 n = (1.0 + sqrtf0) / (1.0 - sqrtf0);
  const vec3 c = vdoth.xxx;
  const vec3 g = sqrt(n * n + c * c - 1.0);

  const vec3 x = (g - c) / (g + c);
  const vec3 x2 = x * x;
  const vec3 y = ((g + c) * c - 1.0) / ((g - c) * c + 1.0);
  const vec3 y2 = y * y;
  
  return 0.5 * x2 * (1.0 + y2);
}

float GSImplicit(float ndotl, float ndotv)
{
  return ndotl * ndotv;
}

float GSNeumann(float ndotl, float ndotv)
{   
  return (ndotl * ndotv) / max(ndotl, ndotv);
}

float GSCookTorrance(float ndotl, float ndotv, float vdoth, float ndoth)
{
  const float invvdoth = 1.0 / vdoth;
  const float dobndoth = 2.0 * ndoth;
  const float x = (dobndoth * ndotv) * invvdoth;
  const float y = (dobndoth * ndotl) * invvdoth;
  return min(1.0, min(x, y));
}

float GSKelemen(float ndotl, float ndotv, float vdoth)
{
  const float vdoth2 = vdoth * vdoth;
  return (ndotl * ndotv) / vdoth2;
}

float GSSBeckmann(float r2, float ndotv)
{
  const float ndotv2 = ndotv * ndotv;
  const float c = ndotv / (r2 * sqrt(1.0 - ndotv2));
  const float c2 = c * c;
  return c < 1.6 ? ((3.535 * c + 2.181 * c2) / (1.0 + 2.276 * c + 2.577 * c2)) : 1.0;
} 

float GSBeckmann(float r2, float ndotv, float ndotl)
{
  return GSSBeckmann(r2, ndotv) * GSSBeckmann(r2, ndotl);
}

float GSSGGX(float r4, float ndotv)
{
  const float ndotv2 = ndotv * ndotv;

  const float nom   = 2.0 * ndotv;
  const float denom = ndotv + sqrt(r4 + (1.0 - r4) * ndotv2);

  return nom / denom;
}

float GSGGX(float r4, float ndotv, float ndotl)
{
  return GSSGGX(r4, ndotv) * GSSGGX(r4, ndotl);
}

float GSSSchlickBeckmann(float r2, float ndotv)
{
  const float x = sqrt(2.0 * INV_PI);
  const float k = r2 * x;

  const float nom   = ndotv;
  const float denom = ndotv * (1.0 - k) + k;

  return nom / denom;
}

float GSSchlickBeckmann(float r2, float ndotv, float ndotl)
{
  return GSSSchlickBeckmann(r2, ndotv) * GSSSchlickBeckmann(r2, ndotl);
}

float NDFBlinnPhong(float r4, float ndoth)
{
  const float invr4 = 1.0 / r4;
  const float brianKarisPower = 2.0 * invr4 - 2.0;
  return pow(ndoth, brianKarisPower) * INV_PI * invr4;
}

// Beckmann is a physically correct and fast type and should therefore be the preferred method for normal use
float NDFBeckmann(float r4, float ndoth)
{
  const float ndoth2    = ndoth * ndoth;
  const float ndoth4    = ndoth2 * ndoth2;
  const float invndoth2 = 1.0 / ndoth2;
  const float invndoth4 = 1.0 / ndoth4;
  const float invr4     = 1.0 / r4;

  const float expval = (ndoth2 - 1.0) * invr4 * invndoth2;

  return invr4 * INV_PI * invndoth4 * exp(expval);
}

// GGX produces the greatest dispersion and works best for simulating metal surfaces 
float NDFGGXTrowbridgeReitz(float r4, float ndoth)
{
  const float ndoth2 = ndoth * ndoth;
  const float x = ndoth2 * (r4 - 1.0) + 1.0;

  const float nom   = r4;
  const float denom = x * x * PI;

  return nom / denom;
}

vec3 debugCookTorranceBRDFDiffuse(vec3 a, float m, vec3 f0, float ndotl)
{
  vec3 F;
  if (pbrDebug.fsIndex == 0) F = FNone(f0);
  if (pbrDebug.fsIndex == 1) F = FSchlick(f0, ndotl);
  if (pbrDebug.fsIndex == 2) F = FCookTorrance(f0, ndotl);
  if (pbrDebug.fsIndex == 3) F = pbrDebug.fsConstant;
  return a * (1.0 - m) * (1.0 - F) * ndotl * INV_PI;
}

vec3 debugCookTorranceBDRFSpecular(float r2, vec3 f0, float w, 
                                   float ndoth, float ndotl, 
                                   float ndotv, float vdoth)
{
  const float r4 = r2 * r2;

  float D;
  if (pbrDebug.ndfIndex == 0) D = NDFBlinnPhong(r4, ndoth);
  if (pbrDebug.ndfIndex == 1) D = NDFBeckmann(r4, ndoth);
  if (pbrDebug.ndfIndex == 2) D = NDFGGXTrowbridgeReitz(r4, ndoth);
  if (pbrDebug.ndfIndex == 3) D = pbrDebug.ndfConstant;
  float G;
  if (pbrDebug.gsIndex == 0) G = GSImplicit(ndotl, ndotv);
  if (pbrDebug.gsIndex == 1) G = GSNeumann(ndotl, ndotv);
  if (pbrDebug.gsIndex == 2) G = GSCookTorrance(ndotl, ndotv, vdoth, ndoth);
  if (pbrDebug.gsIndex == 3) G = GSKelemen(ndotl, ndotv, ndoth);
  if (pbrDebug.gsIndex == 4) G = GSBeckmann(r2, ndoth, ndotl);
  if (pbrDebug.gsIndex == 5) G = GSGGX(r4, ndotv, ndotl);
  if (pbrDebug.gsIndex == 6) G = GSSchlickBeckmann(r4, ndotv, ndotl);
  if (pbrDebug.gsIndex == 7) G = pbrDebug.gsConstant;
  vec3 F;
  if (pbrDebug.fsIndex == 0) F = FNone(f0);
  if (pbrDebug.fsIndex == 1) F = FSchlick(f0, vdoth);
  if (pbrDebug.fsIndex == 2) F = FCookTorrance(f0, vdoth);
  if (pbrDebug.fsIndex == 3) F = pbrDebug.fsConstant;

  // Mitigate specular highlight being incorrectly brighter than the light source
  const float y = min(1.0, (D * w) / (4.0 * ndotv));
  return y * G * F;
}

vec3 debugCookTorranceBRDF(vec3 I, float w, vec3 a, 
                      float m, float r, vec3 f0, 
                      vec3 n, vec3 v, vec3 l, vec3 h)
{
  const float ndoth = max(dot(n, h), 0.0);
  const float ndotv = max(dot(n, v), 0.0);
  const float ndotl = max(dot(n, l), 0.0);
  const float vdoth = max(dot(v, h), 0.0);

  const vec3 diffuse = debugCookTorranceBRDFDiffuse(a, m, f0, ndotl);
  const vec3 specular = debugCookTorranceBDRFSpecular(r, f0, w, ndoth, ndotl, ndotv, vdoth);
  return I * (pbrDebug.diffCoeff * diffuse + pbrDebug.specCoeff * specular);
}

#endif // CRUDE_SHADERS_PBR_H
