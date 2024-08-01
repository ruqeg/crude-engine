module;

#include <memory>

export module crude.core.memory;

export import crude.core.free_rbt_allocator;
export import crude.core.system_allocator;
export import crude.core.aligned_allocator;
export import crude.core.cxx_allocator_container;

export namespace crude::core
{

void initializeMemory(core::uint32 defaultFreeRBTCapacity) noexcept;
void resetMemory() noexcept;

std::shared_ptr<CXX_Allocator_Container> getDefaultAllocator();
std::shared_ptr<Free_RBT_Allocator> getDefaultFreeRBTAllocator() noexcept;
std::shared_ptr<System_Allocator> getDefaultSystemAllocator() noexcept;
std::shared_ptr<Aligned_Allocator> getDefaultAlignedAllocator() noexcept;

[[nodiscard]] void* defaultAllocate(core::size_t n) noexcept { return getDefaultAllocator()->allocate(n); }
[[nodiscard]] void defaultDeallocate(void* ptr) noexcept { getDefaultAllocator()->deallocate(ptr); }

template<class T, typename... Args>
[[nodiscard]] T* defaultCxxAllocate(Args&&... args) noexcept { return getDefaultAllocator()->cxxAllocate<T>(std::forward<Args>(args)...); }
template<class T, typename... Args>
[[nodiscard]] T* defaultCxxAllocate(size_t n, Args&&... args) noexcept { return getDefaultAllocator()->cxxAllocate<T>(n, std::forward<Args>(args)...); }

template<class T>
void defaultCxxDeallocate(T* ptr) noexcept { return getDefaultAllocator()->cxxDeallocate<T>(ptr); }
template<class T>
void defaultCxxDeallocate(size_t n, T* ptr) noexcept { return getDefaultAllocator()->cxxDeallocate<T>(n, ptr); }

}