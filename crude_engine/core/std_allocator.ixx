module;

export module crude.core.std_allocator;

export import crude.core.memory_manager;

export namespace crude::core
{

template<class T>
struct STD_Allocator
{
  using value_type = T;

  STD_Allocator() = default;

  template<class U>
  constexpr STD_Allocator(const STD_Allocator<U>&) noexcept {}

  [[nodiscard]] T* allocate(size_t n);

  void deallocate(T* ptr, size_t n) noexcept;
};

template<class T, class U>
bool operator==(const STD_Allocator<T>&, const STD_Allocator<U>&);

template<class T, class U>
bool operator!=(const STD_Allocator<T>&, const STD_Allocator<U>&);

template<class T, class U>
bool operator==(const STD_Allocator<T>&, const STD_Allocator<U>&)
{
  return true;
}

template<class T, class U>
bool operator!=(const STD_Allocator<T>&, const STD_Allocator<U>&)
{
  return false;
}

template<class T>
[[nodiscard]] T* STD_Allocator<T>::allocate(size_t n)
{
  T* ptr = static_cast<T*>(Memory_Manager::getDefaultAllocator()->allocate(n * sizeof(T)));
  return ptr;
}

template<class T>
void STD_Allocator<T>::deallocate(T* ptr, size_t n) noexcept
{
  Memory_Manager::getDefaultAllocator()->deallocate(ptr);
}

}