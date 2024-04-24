#pragma once

#include <core/std_allocator.hpp>

#include <unordered_map>
#include <set>
#include <vector>
#include <queue>

namespace crude_engine
{

template<class T, class Allocator = STD_Allocator<T>>
using vector = std::vector<T, Allocator>;

template<class K, class T, class Hash = std::hash<K>, class Cmp = std::equal_to<K>, class Allocator = STD_Allocator<std::pair<const K, T>>>
using unordered_map = std::unordered_map<K, T, Hash, Cmp, Allocator>;

template<class T, class Cmp = std::less<T>, class Allocator = STD_Allocator<T>>
using set = std::set<T, Cmp, Allocator>;

template<class T, class Allocator = STD_Allocator<T>>
using queue = std::queue<T, std::deque<T, Allocator>>;

}