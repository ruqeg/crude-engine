module;

#include <array>
#include <span>
#include <optional>
#include <string>

export module crude.core.std_containers_stack;

export namespace crude::core
{

template<class T, size_t N>
using array = std::array<T, N>;

template<class T, std::size_t N = std::dynamic_extent>
using span = std::span<T, N>;

template<class T1, class T2>
using pair = std::pair<T1, T2>;

template<class T1>
using optional = std::optional<T1>;

using u8string_view = std::u8string_view;
using u16string_view = std::u16string_view;
using u32string_view = std::u32string_view;

constexpr std::nullopt_t nullopt = std::nullopt;

}