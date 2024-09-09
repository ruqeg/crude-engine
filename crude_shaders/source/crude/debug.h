#ifndef CRUDE_SHADERS_DEBUG_H
#define CRUDE_SHADERS_DEBUG_H

struct PBRDebug
{
  float ndfConstant;
  float gsConstant;
  int   ndfIndex;
  int   gsIndex;
  int   fsIndex;
  vec3  fsConstant;
  vec3  diffCoeff;
  vec3  specCoeff;
};

#endif // CRUDE_SHADERS_DEBUG_H
