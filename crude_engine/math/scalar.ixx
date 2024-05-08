module;

export module crude_engine.math.scalar;

import crude_engine.core.alias;

export namespace crude_engine
{

class Scalar
{
public:
  static float32 round(float32 s) noexcept;
  static float32 floor(float32 s) noexcept;
  static float32 ceil(float32 s) noexcept;
  static float32 trunc(float32 s) noexcept;
  static float32 clamp(float32 s, float32 min, float32 max) noexcept;
  static float32 abs(float32 s) noexcept;

  static float32 pow(float32 s1, float32 s2) noexcept;
  static float32 sqrt(float32 s) noexcept;

  static float32 exp2(float32 s) noexcept;
  static float32 exp(float32 s) noexcept;

  static float32 log2(float32 s) noexcept;
  static float32 log10(float32 s) noexcept;
  static float32 log(float32 s) noexcept;

  static float32 sin(float32 s) noexcept;
  static float32 cos(float32 s) noexcept;
  static float32 tan(float32 s) noexcept;
  static float32 arcSin(float32 s) noexcept;
  static float32 arcCos(float32 s) noexcept;
  static float32 arcTan(float32 s) noexcept;
};

}