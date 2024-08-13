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
