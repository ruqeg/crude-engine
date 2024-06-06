module;

export module crude.math.scalar;

import crude.core.alias;

export namespace crude::math
{

namespace scalar
{

core::float32 max(core::float32 s1, core::float32 s2) noexcept;
core::float32 min(core::float32 s1, core::float32 s2) noexcept;
core::float32 round(core::float32 s) noexcept;
core::float32 floor(core::float32 s) noexcept;
core::float32 ceil(core::float32 s) noexcept;
core::float32 trunc(core::float32 s) noexcept;
core::float32 clamp(core::float32 s, core::float32 min, core::float32 max) noexcept;
core::float32 abs(core::float32 s) noexcept;

core::float32 pow(core::float32 s1, core::float32 s2) noexcept;
core::float32 sqrt(core::float32 s) noexcept;

core::float32 exp2(core::float32 s) noexcept;
core::float32 exp(core::float32 s) noexcept;

core::float32 log2(core::float32 s) noexcept;
core::float32 log10(core::float32 s) noexcept;
core::float32 log(core::float32 s) noexcept;

core::float32 sin(core::float32 s) noexcept;
core::float32 cos(core::float32 s) noexcept;
core::float32 tan(core::float32 s) noexcept;
core::float32 arcSin(core::float32 s) noexcept;
core::float32 arcCos(core::float32 s) noexcept;
core::float32 arcTan(core::float32 s) noexcept;

}

}