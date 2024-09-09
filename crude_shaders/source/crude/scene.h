#ifndef CRUDE_SHADERS_MESH_H
#define CRUDE_SHADERS_MESH_H

#include "config.h"

struct Meshlet
{
  uint8_t  vertexCount;
  uint8_t  primitiveCount;
  uint     vertexOffset;
  uint     primitiveOffest;
};

struct Vertex
{
  vec3 position;
  vec3 normal;
  vec2 texCoord;
};

struct Mesh_Lod
{
  uint indexOffset;
  uint indexCount;
  uint meshletOffset;
  uint meshletCount;
};

struct Mesh
{
  uint vertexOffset;
  uint vertexCount;
  uint lodCount;
  Mesh_Lod lods[8];
};

struct Camera
{
  vec3 position;
  mat4 worldToView;
  mat4 viewToClip;
  mat4 clipToView;
  mat4 viewToWorld;
};

struct SubMeshLod
{
  uint indexOffset;
  uint indexCount;
  uint meshletOffset;
  uint meshletCount;
};

struct SubMeshDraw
{
  uint        vertexOffset;
  uint        vertexCount;
  uint        lodCount;
  SubMeshLod  lods[8];
};

struct PointLight
{
  vec3  position;
  float _pad1;
  vec3  intensity;
  float _pad2;
  float sourceRadius;
};

#endif // CRUDE_SHADERS_MESH_H
