module;

#include <memory>

export module crude.core.memory_manager;

export import crude.core.free_rbt_allocator;
export import crude.core.system_allocator;
export import crude.core.aligned_allocator;
export import crude.core.cxx_allocator_container;

export namespace crude::core
{

class Memory_Manager
{
public:
  void initialize(core::uint32 defaultFreeRBTCapacity) noexcept;
  void deinitialize() noexcept;
public:
  static std::shared_ptr<CXX_Allocator_Container> getDefaultAllocator() noexcept { return getInstance().getDefaultFreeRBTAllocator(); }
  static std::shared_ptr<Free_RBT_Allocator> getDefaultFreeRBTAllocator() noexcept { return getInstance().m_defaultFreeRBTAllocators; }
  static std::shared_ptr<System_Allocator> getDefaultSystemAllocator() noexcept { return getInstance().m_defaultSystemAllocators; }
  static std::shared_ptr<Aligned_Allocator> getDefaultAlignedAllocator() noexcept { return getInstance().m_defaultAlignedAllocator; }
public:
  static Memory_Manager& getInstance() noexcept;
private:
  Memory_Manager() = default;
  ~Memory_Manager() = default;
private:
  std::shared_ptr<Free_RBT_Allocator>  m_defaultFreeRBTAllocators;
  std::shared_ptr<System_Allocator>    m_defaultSystemAllocators;
  std::shared_ptr<Aligned_Allocator>   m_defaultAlignedAllocator;
};

[[nodiscard]] void* defaultAllocate(core::size_t n) noexcept
{
  return Memory_Manager::getDefaultAllocator()->allocate(n);
}

[[nodiscard]] void defaultDeallocate(void* ptr) noexcept
{
  Memory_Manager::getDefaultAllocator()->deallocate(ptr);
}

template<class T, typename... Args>
[[nodiscard]] T* defaultCxxAllocate(Args&&... args) noexcept
{
  return Memory_Manager::getDefaultAllocator()->cxxAllocate<T>(std::forward<Args>(args)...);
}

template<class T, typename... Args>
[[nodiscard]] T* defaultCxxAllocate(size_t n, Args&&... args) noexcept
{
  return Memory_Manager::getDefaultAllocator()->cxxAllocate<T>(n, std::forward<Args>(args)...);
}

template<class T>
void defaultCxxDeallocate(T* ptr) noexcept
{
  return Memory_Manager::getDefaultAllocator()->cxxDeallocate<T>(ptr);
}

template<class T>
void defaultCxxDeallocate(size_t n, T* ptr) noexcept
{
  return Memory_Manager::getDefaultAllocator()->cxxDeallocate<T>(n, ptr);
}

}