module;

#include <cstdint>
#include <cstddef>
#include <uchar.h>

export module crude_engine.core.alias;

export namespace crude_engine
{

using byte = uint8_t;

using int8  = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8  = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using float32 = float;
using float64 = double;

using char8  = char;
using char16 = wchar_t;

using bool8  = int8;
using bool16 = int16;
using bool32 = int32;
using bool64 = int64;

using size_t = std::size_t;

using nullptr_t = decltype(nullptr);

}
