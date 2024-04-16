#include <math/scalar.hpp>
#include <math.h>

namespace crude_engine
{

float32 Scalar::round(float32 s) noexcept
{
  float32 x = Scalar::floor(s);
  float32 y = s - x;
  float32 z;
  if (y < 0.5f)
  {
    z = x;
  }
  else if (y > 0.5f)
  {
    z = x + 1.f;
  }
  else
  {
    z = x;
  }
  return z;
}

float32 Scalar::floor(float32 s) noexcept
{
  float32 y = static_cast<int64>(s);
  float32 x;
  
  if (s > 0.0f)
  {
    x = y;
  }
  else
  {
    x = y - 1;
  }
  
  return x;
}

float32 Scalar::ceil(float32 s) noexcept
{
  float32 y = static_cast<int64>(s);
  float32 x;

  if (s > 0.0f)
  {
    x = y + 1.0f;
  }
  else
  {
    x = y;
  }

  return x;
}

float32 Scalar::trunc(float32 s) noexcept
{
  float32 x = static_cast<int64>(s);
  return x;
}

float32 Scalar::clamp(float32 s, float32 min, float32 max) noexcept
{
  float32 x;
  if (s < min)
  {
    x = min;
  }
  else if (s > max)
  {
    x = max;
  }
  else
  {
    x = s;
  }
  
  return x;
}

float32 Scalar::abs(float32 s) noexcept
{
  float32 x;
  if (s < 0)
  {
    x = -s;
  }
  else
  {
    x = s;
  }
  return x;
}

float32 Scalar::pow(float32 s1, float32 s2) noexcept
{
  float32 x = powf(s1, s2);
  return x;
}

float32 Scalar::sqrt(float32 s) noexcept
{
  float32 x = sqrtf(s);
  return x;
}

float32 Scalar::exp2(float32 s) noexcept
{
  float32 x = exp2f(s);
  return x;
}

float32 Scalar::exp(float32 s) noexcept
{
  float32 x = expf(s);
  return x;
}

float32 Scalar::log2(float32 s) noexcept
{
  float32 x = log2f(s);
  return x;
}

float32 Scalar::log10(float32 s) noexcept
{
  float32 x = log10f(s);
  return x;
}

float32 Scalar::log(float32 s) noexcept
{
  float32 x = logf(s);
  return x;
}

float32 Scalar::sin(float32 s) noexcept
{
  float32 x = sinf(s);
  return x;
}

float32 Scalar::cos(float32 s) noexcept
{
  float32 x = cosf(s);
  return x;
}

float32 Scalar::tan(float32 s) noexcept
{
  float32 x = tanf(s);
  return x;
}

float32 Scalar::arcSin(float32 s) noexcept
{
  float32 x = asinf(s);
  return x;
}

float32 Scalar::arcCos(float32 s) noexcept
{
  float32 x = acosf(s);
  return x;
}

float32 Scalar::arcTan(float32 s) noexcept
{
  float32 x = atanf(s);
  return x;
}

}