module;

export module crude.math.convert;

import crude.math.matrix;
import crude.math.fuicont;

export namespace crude::math
{

void storeFloat3(Float3& f, Vector v);
void storeFloat4(Float4& f, Vector v);
void storeFloat4x4(Float4x4& f, Matrix m);
Vector loadFloat3(const Float3& f);
Vector loadFloat4(const Float4& f);
Matrix loadFloat4x4(const Float4x4& f);

}