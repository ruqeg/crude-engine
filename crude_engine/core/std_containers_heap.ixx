module;

#include <memory>
#include <unordered_map>
#include <set>
#include <vector>
#include <deque>
#include <queue>

export module crude.core.std_containers_heap;

export import crude.core.std_allocator;

export namespace crude::core
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

template<class T>
using shared_ptr = std::shared_ptr<T>;

template <class T, class ...Args>
shared_ptr<T> allocateShared(Args&&... args) noexcept
{
  return std::allocate_shared<T>(STD_Allocator<T>(), std::forward<Args>(args)...);
}

}