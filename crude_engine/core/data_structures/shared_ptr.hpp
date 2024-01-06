#pragma once

#include <core/memory/memory_system.hpp>
#include <iterator>

namespace crude_engine
{

// !TODO make something with allocator
template<class T, class Allocator = Memory_System::Default_Allocator>
class Shared_Ptr
{
public:
  using Ref_Count         = uint32;
  using Ref_Count_Pointer = Ref_Count*;
public:
  Shared_Ptr() noexcept;
  ~Shared_Ptr() noexcept;
  template<typename... Args>
  Shared_Ptr(Args&&... args) noexcept;
  Shared_Ptr(const T& value) noexcept;
  Shared_Ptr(const Shared_Ptr& other) noexcept;
  Shared_Ptr(Shared_Ptr&& other) noexcept;

  CRUDE_INLINE Shared_Ptr& operator=(const Shared_Ptr& other) noexcept;
  CRUDE_INLINE Shared_Ptr& operator=(Shared_Ptr&& other) noexcept;

  CRUDE_INLINE bool operator==(const Shared_Ptr& other) const noexcept;
  CRUDE_INLINE bool operator!=(const Shared_Ptr& other) const noexcept;

  CRUDE_INLINE T& operator*() noexcept;
  CRUDE_INLINE T* operator->() noexcept;

  CRUDE_INLINE const T& operator*() const noexcept;
  CRUDE_INLINE const T* operator->() const noexcept;

protected:
  CRUDE_INLINE T* getPtr() noexcept;
  CRUDE_INLINE Ref_Count_Pointer getRefCount() noexcept;
  CRUDE_INLINE CRUDE_NODISCARD byte* allocateMemBlock() noexcept;

public:
  template<typename... Args>
  static Shared_Ptr<T, Allocator> makeShared(Args&&... args);

protected:
  byte* m_memBlock;
};

template<class T, class Allocator>
Shared_Ptr<T, Allocator>::Shared_Ptr() noexcept
  :
  m_memBlock(nullptr)
{}

template<class T, class Allocator>
Shared_Ptr<T, Allocator>::~Shared_Ptr() noexcept
{
  if (m_memBlock == nullptr)
    return;

  (*getRefCount())--;
  
  if (*getRefCount() == 0)
  {
    Memory_Utils::destructorAt(getPtr());
    Allocator::free(m_memBlock);
  }
}

template<class T, class Allocator>
template<typename ...Args>
Shared_Ptr<T, Allocator>::Shared_Ptr(Args&& ...args) noexcept
{
  m_memBlock = allocateMemBlock();
  Memory_Utils::constructAt(getPtr(), Utility::forward<Args>(args)...);
  (*getRefCount()) = 1;
}

template<class T, class Allocator>
Shared_Ptr<T, Allocator>::Shared_Ptr(const T& value) noexcept
{
  m_memBlock = allocateMemBlock();
  Memory_Utils::constructAt(getPtr(), value);
  (*getRefCount()) = 1;
}

template<class T, class Allocator>
Shared_Ptr<T, Allocator>::Shared_Ptr(const Shared_Ptr& other) noexcept
{
  m_memBlock = other.m_memBlock;

  if (m_memBlock)
  {
    *(getRefCount()) += 1;
  }
}

template<class T, class Allocator>
Shared_Ptr<T, Allocator>::Shared_Ptr(Shared_Ptr&& other) noexcept
{
  m_memBlock = other.m_memBlock;
  other.m_memBlock = nullptr;
}

template<class T, class Allocator>
Shared_Ptr<T, Allocator>& Shared_Ptr<T, Allocator>::operator=(const Shared_Ptr<T, Allocator>& other) noexcept
{
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
  m_memBlock = other.m_memBlock;
  other.m_memBlock = nullptr;
  return *this;
}

template<class T, class Allocator>
bool Shared_Ptr<T, Allocator>::operator==(const Shared_Ptr<T, Allocator>& other) const noexcept
{
  return (m_memBlock == other.m_memBlock);
}

template<class T, class Allocator>
bool Shared_Ptr<T, Allocator>::operator!=(const Shared_Ptr<T, Allocator>& other) const noexcept
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
const T& Shared_Ptr<T, Allocator>::operator*() const noexcept
{
  return *getPtr();
}

template<class T, class Allocator>
const T* Shared_Ptr<T, Allocator>::operator->() const noexcept
{
  return getPtr();
}

template<class T, class Allocator>
T* Shared_Ptr<T, Allocator>::getPtr() noexcept
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
template<typename ...Args>
Shared_Ptr<T, Allocator> Shared_Ptr<T, Allocator>::makeShared(Args&& ...args)
{
  Shared_Ptr<T, Allocator> shared(Utility::forward<Args>(args)...);
  return shared;
}

} // namespace crude_engine

#include <core/data_structures/shared_ptr.inl>