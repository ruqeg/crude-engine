module;

#include <unordered_map>
#include <set>
#include <vector>
#include <deque>
#include <queue>

export module crude_engine.core.std_containers_heap;

export import crude_engine.core.std_allocator;

export namespace crude_engine
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

}