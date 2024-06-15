module;

#include <array>
#include <span>

export module crude.core.std_containers_stack;

export namespace crude::core
{

template<class T, size_t N>
using array = std::array<T, N>;

template<class T, std::size_t N = std::dynamic_extent>
using span = std::span<T, N>;

template<class T1, class T2>
using pair = std::pair<T1, T2>;

}