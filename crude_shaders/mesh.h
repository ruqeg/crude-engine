#include "config.h"

struct Vertex
{
  vec3 position;
  vec3 normal;
  vec2 texcoord;
};

struct Meshlet
{
	uint vertexOffset;
	uint primitiveOffset;
	uint vertexCount;
	uint primitiveCount;
};

struct Camera
{
  mat4 worldToClip;
  mat4 worldToView;
  mat4 viewToClip;
  mat4 clipToView;
  mat4 viewToWorld;
  mat4 clipToWorld;
};


struct PerFrame
{
  Camera camera;
};

struct MeshLod
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
	MeshLod lods[8];
};

struct MeshDraw
{
	vec3 position;
	float scale;
	vec4 orientation;
	uint meshIndex;
	uint vertexOffset;
	uint meshletVisibilityOffset;
};

struct MeshDrawCommand
{
	uint drawId;
	uint indexCount;
	uint instanceCount;
	uint firstIndex;
	uint vertexOffset;
	uint firstInstance;
};

struct MeshTaskCommand
{
	uint drawId;
	uint taskOffset;
	uint taskCount;
	uint lateDrawVisibility;
	uint meshletVisibilityOffset;
};

struct MeshTaskPayload
{
	uint drawId;
	uint meshletIndices[TASK_WGSIZE];
};
