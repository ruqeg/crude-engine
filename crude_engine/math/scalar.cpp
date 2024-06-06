#include <math.h>

module crude.math.scalar;

namespace crude::math
{

core::float32 scalar::max(core::float32 s1, core::float32 s2) noexcept
{
  core::float32 z;
  if (s1 > s2)
  {
    z = s1;
  }
  else
  {
    z = s2;
  }
  return z;
}

core::float32 scalar::min(core::float32 s1, core::float32 s2) noexcept
{
  core::float32 z;
  if (s1 < s2)
  {
    z = s1;
  }
  else
  {
    z = s2;
  }
  return z;
}

core::float32 scalar::round(core::float32 s) noexcept
{
  core::float32 x = floor(s);
  core::float32 y = s - x;
  core::float32 z;
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

core::float32 scalar::floor(core::float32 s) noexcept
{
  core::float32 y = static_cast<core::int64>(s);
  core::float32 x;
  
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

core::float32 scalar::ceil(core::float32 s) noexcept
{
  core::float32 y = static_cast<core::int64>(s);
  core::float32 x;

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

core::float32 scalar::trunc(core::float32 s) noexcept
{
  core::float32 x = static_cast<core::int64>(s);
  return x;
}

core::float32 scalar::clamp(core::float32 s, core::float32 min, core::float32 max) noexcept
{
  core::float32 x;
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

core::float32 scalar::abs(core::float32 s) noexcept
{
  core::float32 x;
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

core::float32 scalar::pow(core::float32 s1, core::float32 s2) noexcept
{
  core::float32 x = powf(s1, s2);
  return x;
}

core::float32 scalar::sqrt(core::float32 s) noexcept
{
  core::float32 x = sqrtf(s);
  return x;
}

core::float32 scalar::exp2(core::float32 s) noexcept
{
  core::float32 x = exp2f(s);
  return x;
}

core::float32 scalar::exp(core::float32 s) noexcept
{
  core::float32 x = expf(s);
  return x;
}

core::float32 scalar::log2(core::float32 s) noexcept
{
  core::float32 x = log2f(s);
  return x;
}

core::float32 scalar::log10(core::float32 s) noexcept
{
  core::float32 x = log10f(s);
  return x;
}

core::float32 scalar::log(core::float32 s) noexcept
{
  core::float32 x = logf(s);
  return x;
}

core::float32 scalar::sin(core::float32 s) noexcept
{
  core::float32 x = sinf(s);
  return x;
}

core::float32 scalar::cos(core::float32 s) noexcept
{
  core::float32 x = cosf(s);
  return x;
}

core::float32 scalar::tan(core::float32 s) noexcept
{
  core::float32 x = tanf(s);
  return x;
}

core::float32 scalar::arcSin(core::float32 s) noexcept
{
  core::float32 x = asinf(s);
  return x;
}

core::float32 scalar::arcCos(core::float32 s) noexcept
{
  core::float32 x = acosf(s);
  return x;
}

core::float32 scalar::arcTan(core::float32 s) noexcept
{
  core::float32 x = atanf(s);
  return x;
}

}