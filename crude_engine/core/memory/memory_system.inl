#include <core/memory/memory_system.hpp>

namespace crude_engine
{

template<class T, typename... Args>
T* Memory_System::mnewFRBTA(std::size_t n, Args&&... args) noexcept
{
  return Memory_Manager::getInstance().getFreeRBTAllocators().mnew<T>(n, Utility::forward<Args>(args)...);
}

template<class T>
void Memory_System::mdeleteFRBTA(T* ptr) noexcept
{
  Memory_Manager::getInstance().getFreeRBTAllocators().mdelete<T>(ptr);
}

}  // namespace crude_engine