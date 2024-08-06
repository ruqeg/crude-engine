module;

#include <memory>
#include <unordered_map>
#include <set>
#include <vector>
#include <deque>
#include <queue>
#include <string>

export module crude.core.std_containers_heap;

export import crude.core.alias;
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

using u8string = std::basic_string<core::char8, std::char_traits<core::char8>, STD_Allocator<core::char8>>;
using u16string = std::basic_string<core::char16, std::char_traits<core::char16>, STD_Allocator<core::char16>>;
using u32string = std::basic_string<core::char32, std::char_traits<core::char32>, STD_Allocator<core::char32>>;

template<class T>
using shared_ptr = std::shared_ptr<T>;

template <class T>
shared_ptr<T> allocateShared() noexcept
{
  return std::allocate_shared<T>(STD_Allocator<T>());
}

template <class T>
shared_ptr<T> allocateShared(core::size_t n) noexcept
{
  return std::allocate_shared<T>(STD_Allocator<T>(), n);
}

template <class T, class ...Args>
shared_ptr<T> allocateShared(Args&&... args) noexcept
{
  return std::allocate_shared<T>(STD_Allocator<T>(), std::forward<Args>(args)...);
}

// !TODO idk, just need it for graphics T_T
template <class T>
std::shared_ptr<T> makeUnsafeSharedWithEmptyDestructor(T* ptr)
{
  return std::shared_ptr<T>(ptr, [](T*){});
}

}