#pragma once

#include <core/std_allocator.hpp>

#include <unordered_map>
#include <set>
#include <vector>
#include <deque>
#include <queue>
#include <array>
#include <span>

namespace crude_engine
{

template<class T, class Allocator = STD_Allocator<T>>
using vector = std::vector<T, Allocator>;

template<class K, class T, class Hash = std::hash<K>, class Cmp = std::equal_to<K>, class Allocator = STD_Allocator<std::pair<const K, T>>>
using unordered_map = std::unordered_map<K, T, Hash, Cmp, Allocator>;

template<class T, class Cmp = std::less<T>, class Allocator = STD_Allocator<T>>
using set = std::set<T, Cmp, Allocator>;

template<class T, class Allocator = STD_Allocator<T>>
using deque = std::deque<T, Allocator>;

template<class T, class Allocator = STD_Allocator<T>>
using queue = std::queue<T, deque<T,  Allocator>>;

template<class T, size_t N>
using array = std::array<T, N>;

template<class T, std::size_t N = std::dynamic_extent>
using span = std::span<T, N>;

}