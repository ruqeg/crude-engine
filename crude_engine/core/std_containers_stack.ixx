module;

#include <array>
#include <span>

export module crude_engine.std_containers_stack;

export namespace crude_engine
{

template<class T, size_t N>
using array = std::array<T, N>;

template<class T, std::size_t N = std::dynamic_extent>
using span = std::span<T, N>;

}