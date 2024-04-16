#include <core/shared_ptr.hpp>

namespace crude_engine
{

template<class U, class UAllocator = Memory_System::Default_Allocator, typename... Args>
extern Shared_Ptr<U, UAllocator> makeShared(Args&&... args) noexcept;

template<class T, class Allocator>
Shared_Ptr<T, Allocator>::Shared_Ptr() noexcept
  :
  m_memBlock(nullptr)
{}

template<class T, class Allocator>
Shared_Ptr<T, Allocator>::Shared_Ptr(nullptr_t) noexcept
  :
  m_memBlock(nullptr)
{}

template<class T, class Allocator>
Shared_Ptr<T, Allocator>::~Shared_Ptr() noexcept
{
  release();
}

template<class T, class Allocator>
Shared_Ptr<T, Allocator>::Shared_Ptr(const T& value) noexcept
{
  m_memBlock = allocateMemBlock();
  Utility::constructAt(getPtr(), value);
  (*getRefCount()) = 1;
}

template<class T, class Allocator>
template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
Shared_Ptr<T, Allocator>::Shared_Ptr(const Shared_Ptr<U, UAllocator>& other) noexcept
{
  m_memBlock = other.m_memBlock;

  if (m_memBlock)
  {
    *(getRefCount()) += 1;
  }
}

template<class T, class Allocator>
template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
Shared_Ptr<T, Allocator>::Shared_Ptr(Shared_Ptr<U, UAllocator>&& other) noexcept
{
  m_memBlock = other.m_memBlock;
  other.m_memBlock = nullptr;
}

template<class T, class Allocator>
template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
Shared_Ptr<T, Allocator>& Shared_Ptr<T, Allocator>::operator=(const Shared_Ptr<U, UAllocator>& other) noexcept
{
  release();

  m_memBlock = other.m_memBlock;

  if (m_memBlock)
  {
    *(getRefCount()) += 1;
  }
  return *this;
}

template<class T, class Allocator>
template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
Shared_Ptr<T, Allocator>& Shared_Ptr<T, Allocator>::operator=(Shared_Ptr<U, UAllocator>&& other) noexcept
{
  release();
  m_memBlock = other.m_memBlock;
  other.m_memBlock = nullptr;
  return *this;
}

template<class T, class Allocator>
Shared_Ptr<T, Allocator>::Shared_Ptr(const Shared_Ptr<T, Allocator>& other) noexcept
{
  m_memBlock = other.m_memBlock;

  if (m_memBlock)
  {
    *(getRefCount()) += 1;
  }
}

template<class T, class Allocator>
Shared_Ptr<T, Allocator>::Shared_Ptr(Shared_Ptr<T, Allocator>&& other) noexcept
{
  m_memBlock = other.m_memBlock;
  other.m_memBlock = nullptr;
}

template<class T, class Allocator>
Shared_Ptr<T, Allocator>& Shared_Ptr<T, Allocator>::operator=(const Shared_Ptr<T, Allocator>& other) noexcept
{
  release();
  m_memBlock = other.m_memBlock;

  if (m_memBlock)
  {
    *(getRefCount()) += 1;
  }
  return *this;
}

template<class T, class Allocator>
Shared_Ptr<T, Allocator>& Shared_Ptr<T, Allocator>::operator=(Shared_Ptr<T, Allocator>&& other) noexcept
{
  release();
  m_memBlock = other.m_memBlock;
  other.m_memBlock = nullptr;
  return *this;
}

template<class T, class Allocator>
template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
bool Shared_Ptr<T, Allocator>::operator==(const Shared_Ptr<U, UAllocator>& other) const noexcept
{
  return (m_memBlock == other.m_memBlock);
}

template<class T, class Allocator>
template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
bool Shared_Ptr<T, Allocator>::operator!=(const Shared_Ptr<U, UAllocator>& other) const noexcept
{
  return (m_memBlock != other.m_memBlock);
}

template<class T, class Allocator>
T& Shared_Ptr<T, Allocator>::operator*() noexcept
{
  return *getPtr();
}

template<class T, class Allocator>
T* Shared_Ptr<T, Allocator>::operator->() noexcept
{
  return getPtr();
}

template<class T, class Allocator>
T& Shared_Ptr<T, Allocator>::operator*() const noexcept
{
  return *getPtr();
}

template<class T, class Allocator>
T* Shared_Ptr<T, Allocator>::operator->() const noexcept
{
  return getPtr();
}

template<class T, class Allocator>
Shared_Ptr<T, Allocator>::operator bool() const noexcept
{
  return (m_memBlock != nullptr);
}

template<class T, class Allocator>
T* Shared_Ptr<T, Allocator>::getPtr() noexcept
{
  return reinterpret_cast<T*>(m_memBlock + sizeof(Ref_Count));
}

template<class T, class Allocator>
T* Shared_Ptr<T, Allocator>::getPtr() const noexcept
{
  return reinterpret_cast<T*>(m_memBlock + sizeof(Ref_Count));
}

template<class T, class Allocator>
Shared_Ptr<T, Allocator>::Ref_Count_Pointer Shared_Ptr<T, Allocator>::getRefCount() noexcept
{
  return reinterpret_cast<Ref_Count_Pointer>(m_memBlock);
}

template<class T, class Allocator>
byte* Shared_Ptr<T, Allocator>::allocateMemBlock() noexcept
{
  return reinterpret_cast<byte*>(Allocator::allocate(sizeof(T) + sizeof(Ref_Count)));
}

template<class T, class Allocator>
CRUDE_INLINE void Shared_Ptr<T, Allocator>::release() noexcept
{
  if (m_memBlock == nullptr)
    return;

  CRUDE_ASSERT(*getRefCount() != 0);

  (*getRefCount())--;

  if (*getRefCount() == 0)
  {
    Utility::destructorAt(getPtr());
    Allocator::free(m_memBlock);
    m_memBlock = nullptr;
  }
  return;
}

template<class U, class UAllocator, typename... Args>
Shared_Ptr<U, UAllocator> makeShared(Args&&... args) noexcept
{
  Shared_Ptr<U, UAllocator> impl;

  impl.m_memBlock = impl.allocateMemBlock();
  Utility::constructAt(impl.getPtr(), Utility::forward<Args>(args)...);
  (*impl.getRefCount()) = 1;

  return impl;
}

}