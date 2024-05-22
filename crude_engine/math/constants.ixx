module;

export module crude.math.constants;

import crude.core.alias;

export namespace crude::math
{

class Constans
{
public:
  static constexpr core::float32 C2PI      = 6.2831853f;
  static constexpr core::float32 CPI       = 3.1415927f;
  static constexpr core::float32 CPI2      = 1.5707964f;
  static constexpr core::float32 CPI4      = 0.7853982f;
  static constexpr core::float32 C1DIVPI   = 0.31830987f;
  static constexpr core::float32 C1DIV2PI  = 0.15915494f;
  static constexpr core::float32 C2DIVPI   = 0.63661976f;

  static constexpr core::float32 CMAXF32   = 3.40282e+38;
  static constexpr core::float32 CMINF32   = 1.17549e-38;
};

}