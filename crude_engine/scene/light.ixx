module;

#include <DirectXMath.h>

export module crude.scene.light;

export import crude.core.alias;

export namespace crude::scene
{

struct Point_Light_CPU
{
  DirectX::XMFLOAT3 intensity;
  core::float32     sourceRadius;
};

struct Point_Light_GPU
{
  DirectX::XMFLOAT3A position;
  DirectX::XMFLOAT3A intensity;
  core::float32      sourceRadius;
};

}