module crude.math.convert;

namespace crude::math
{

void storeFloat3(Float3& f, Vector v)
{
  f.x = svector::getX(v);
  f.y = svector::getY(v);
  f.z = svector::getZ(v);
}

void storeFloat4(Float4& f, Vector v)
{
  f.x = svector::getX(v);
  f.y = svector::getY(v);
  f.z = svector::getZ(v);
  f.w = svector::getW(v);
}

void storeFloat4x4(Float4x4& f, Matrix m)
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

Vector loadFloat3(const Float3& f)
{
  Vector v = svector::set(f.x, f.y, f.z, 0.0);
  return v;
}

Vector loadFloat4(const Float4& f)
{
  Vector v = svector::set(f.x, f.y, f.z, f.w);
  return v;
}

Matrix loadFloat4x4(const Float4x4& f)
{
  Matrix m = smatrix::set(f._00, f._01, f._02, f._03, 
                          f._10, f._11, f._12, f._13,
                          f._20, f._21, f._22, f._23,
                          f._30, f._31, f._32, f._33);
  return m;
}

}