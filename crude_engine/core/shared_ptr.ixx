module;

export module crude_engine.core.shared_ptr;

export import crude_engine.core.memory_system;
export import crude_engine.core.alias;

import crude_engine.core.assert;
import crude_engine.core.utility;

export namespace crude_engine
{

// !TODO make something with allocator (as member, not template0
template<class T, class Allocator = Memory_System::Default_Allocator>
class Shared_Ptr
{
public:
  using Ref_Count         = uint32;
  using Ref_Count_Pointer = Ref_Count*;
public:
  Shared_Ptr() noexcept;
  ~Shared_Ptr() noexcept;
  Shared_Ptr(nullptr_t) noexcept;
  Shared_Ptr(const T& value) noexcept;
  template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
  Shared_Ptr(const Shared_Ptr<U, UAllocator>& other) noexcept;
  template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
  Shared_Ptr(Shared_Ptr<U, UAllocator>&& other) noexcept;
  Shared_Ptr(const Shared_Ptr<T, Allocator>& other) noexcept;
  Shared_Ptr(Shared_Ptr<T, Allocator>&& other) noexcept;

  template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
  inline Shared_Ptr<T, Allocator>& operator=(const Shared_Ptr<U, UAllocator>& other) noexcept;
  template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
  inline Shared_Ptr<T, Allocator>& operator=(Shared_Ptr<U, UAllocator>&& other) noexcept;
  inline Shared_Ptr<T, Allocator>& operator=(const Shared_Ptr<T, Allocator>& other) noexcept;
  inline Shared_Ptr<T, Allocator>& operator=(Shared_Ptr<T, Allocator>&& other) noexcept;

  template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
  inline bool operator==(const Shared_Ptr<U, UAllocator>& other) const noexcept;
  template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
  inline bool operator!=(const Shared_Ptr<U, UAllocator>& other) const noexcept;

  inline T& operator*() noexcept;
  inline T* operator->() noexcept;

  inline T& operator*() const noexcept;
  inline T* operator->() const noexcept;

  explicit operator bool() const noexcept;

protected:
  inline T* getPtr() noexcept;
  inline T* getPtr() const noexcept;
  inline Ref_Count_Pointer getRefCount() noexcept;
  inline [[nodiscard]] byte* allocateMemBlock() noexcept;
  inline void release() noexcept;

protected:
  byte* m_memBlock;

  template<class U, class UAllocator, typename... Args>
  friend Shared_Ptr<U, UAllocator> makeShared(Args&&... args) noexcept;

  template<typename U, class UAllocator>
  friend class Shared_Ptr;
};

template<class U, class UAllocator = Memory_System::Default_Allocator, typename... Args>
Shared_Ptr<U, UAllocator> makeShared(Args&&... args) noexcept;

} // namespace crude_engine

namespace crude_engine
{

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
  constructAt(getPtr(), value);
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
void Shared_Ptr<T, Allocator>::release() noexcept
{
  if (m_memBlock == nullptr)
    return;

  assert(*getRefCount() != 0);

  (*getRefCount())--;

  if (*getRefCount() == 0)
  {
    destructorAt(getPtr());
    Allocator::deallocate(m_memBlock);
    m_memBlock = nullptr;
  }
  return;
}

template<class U, class UAllocator, typename... Args>
Shared_Ptr<U, UAllocator> makeShared(Args&&... args) noexcept
{
  Shared_Ptr<U, UAllocator> impl;

  impl.m_memBlock = impl.allocateMemBlock();
  constructAt(impl.getPtr(), forward<Args>(args)...);
  (*impl.getRefCount()) = 1;

  return impl;
}

}