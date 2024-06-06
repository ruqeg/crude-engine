module crude.math.convert;

namespace crude::math
{

void storeFloat4x4(FLOAT4X4& f, Matrix m)
{
  f.m[0][0] = m.r[0].vector4_f32[0];
  f.m[0][1] = m.r[0].vector4_f32[1];
  f.m[0][2] = m.r[0].vector4_f32[2];
  f.m[0][3] = m.r[0].vector4_f32[3];

  f.m[1][0] = m.r[1].vector4_f32[0];
  f.m[1][1] = m.r[1].vector4_f32[1];
  f.m[1][2] = m.r[1].vector4_f32[2];
  f.m[1][3] = m.r[1].vector4_f32[3];

  f.m[2][0] = m.r[2].vector4_f32[0];
  f.m[2][1] = m.r[2].vector4_f32[1];
  f.m[2][2] = m.r[2].vector4_f32[2];
  f.m[2][3] = m.r[2].vector4_f32[3];

  f.m[3][0] = m.r[3].vector4_f32[0];
  f.m[3][1] = m.r[3].vector4_f32[1];
  f.m[3][2] = m.r[3].vector4_f32[2];
  f.m[3][3] = m.r[3].vector4_f32[3];
}

}