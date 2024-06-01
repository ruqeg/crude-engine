module;

#include <utility>

export module crude.core.shared_ptr;

export import crude.core.memory_manager;
export import crude.core.alias;

import crude.core.assert;
import crude.core.utility;

export namespace crude::core
{

// !TODO make something with allocator 
template<class T>
class Shared_Ptr
{
public:
  using Ref_Count = uint32;
  using Ref_Count_Pointer = Ref_Count*;
public:
  Shared_Ptr() noexcept;
  ~Shared_Ptr() noexcept;
  Shared_Ptr(nullptr_t) noexcept;
  Shared_Ptr(const T& value) noexcept;
  template<class U>
  Shared_Ptr(const Shared_Ptr<U>& other) noexcept;
  template<class U>
  Shared_Ptr(Shared_Ptr<U>&& other) noexcept;
  Shared_Ptr(const Shared_Ptr<T>& other) noexcept;
  Shared_Ptr(Shared_Ptr<T>&& other) noexcept;

  template<class U>
  inline Shared_Ptr<T>& operator=(const Shared_Ptr<U>& other) noexcept;
  template<class U>
  inline Shared_Ptr<T>& operator=(Shared_Ptr<U>&& other) noexcept;
  inline Shared_Ptr<T>& operator=(const Shared_Ptr<T>& other) noexcept;
  inline Shared_Ptr<T>& operator=(Shared_Ptr<T>&& other) noexcept;

  template<class U>
  inline bool operator==(const Shared_Ptr<U>& other) const noexcept;
  template<class U>
  inline bool operator!=(const Shared_Ptr<U>& other) const noexcept;

  inline T& operator*() noexcept;
  inline T* operator->() noexcept;

  inline T& operator*() const noexcept;
  inline T* operator->() const noexcept;

  explicit operator bool() const noexcept;

  bool valid() const noexcept;

protected:
  inline T* getPtr() noexcept;
  inline T* getPtr() const noexcept;
  inline Ref_Count_Pointer getRefCount() noexcept;
  inline [[nodiscard]] byte* allocateMemBlock() noexcept;
  inline void release() noexcept;

protected:
  byte* m_memBlock;

  template<class U, typename... Args>
  friend Shared_Ptr<U> makeShared(Args&&... args) noexcept;

  template<typename U>
  friend class Shared_Ptr;
};

template<class U, typename... Args>
Shared_Ptr<U> makeShared(Args&&... args) noexcept;

template<class T>
Shared_Ptr<T>::Shared_Ptr() noexcept
  :
  m_memBlock(nullptr)
{}

template<class T>
Shared_Ptr<T>::Shared_Ptr(nullptr_t) noexcept
  :
  m_memBlock(nullptr)
{}

template<class T>
Shared_Ptr<T>::~Shared_Ptr() noexcept
{
  release();
}

template<class T>
Shared_Ptr<T>::Shared_Ptr(const T& value) noexcept
{
  m_memBlock = allocateMemBlock();
  constructAt(getPtr(), value);
  (*getRefCount()) = 1;
}

template<class T>
template<class U>
Shared_Ptr<T>::Shared_Ptr(const Shared_Ptr<U>& other) noexcept
{
  m_memBlock = other.m_memBlock;

  if (m_memBlock)
  {
    *(getRefCount()) += 1;
  }
}

template<class T>
template<class U>
Shared_Ptr<T>::Shared_Ptr(Shared_Ptr<U>&& other) noexcept
{
  m_memBlock = other.m_memBlock;
  other.m_memBlock = nullptr;
}

template<class T>
template<class U>
Shared_Ptr<T>& Shared_Ptr<T>::operator=(const Shared_Ptr<U>& other) noexcept
{
  release();

  m_memBlock = other.m_memBlock;

  if (m_memBlock)
  {
    *(getRefCount()) += 1;
  }
  return *this;
}

template<class T>
template<class U>
Shared_Ptr<T>& Shared_Ptr<T>::operator=(Shared_Ptr<U>&& other) noexcept
{
  release();
  m_memBlock = other.m_memBlock;
  other.m_memBlock = nullptr;
  return *this;
}

template<class T>
Shared_Ptr<T>::Shared_Ptr(const Shared_Ptr<T>& other) noexcept
{
  m_memBlock = other.m_memBlock;

  if (m_memBlock)
  {
    *(getRefCount()) += 1;
  }
}

template<class T>
Shared_Ptr<T>::Shared_Ptr(Shared_Ptr<T>&& other) noexcept
{
  m_memBlock = other.m_memBlock;
  other.m_memBlock = nullptr;
}

template<class T>
Shared_Ptr<T>& Shared_Ptr<T>::operator=(const Shared_Ptr<T>& other) noexcept
{
  release();
  m_memBlock = other.m_memBlock;

  if (m_memBlock)
  {
    *(getRefCount()) += 1;
  }
  return *this;
}

template<class T>
Shared_Ptr<T>& Shared_Ptr<T>::operator=(Shared_Ptr<T>&& other) noexcept
{
  release();
  m_memBlock = other.m_memBlock;
  other.m_memBlock = nullptr;
  return *this;
}

template<class T>
template<class U>
bool Shared_Ptr<T>::operator==(const Shared_Ptr<U>& other) const noexcept
{
  return (m_memBlock == other.m_memBlock);
}

template<class T>
template<class U>
bool Shared_Ptr<T>::operator!=(const Shared_Ptr<U>& other) const noexcept
{
  return (m_memBlock != other.m_memBlock);
}

template<class T>
T& Shared_Ptr<T>::operator*() noexcept
{
  return *getPtr();
}

template<class T>
T* Shared_Ptr<T>::operator->() noexcept
{
  return getPtr();
}

template<class T>
T& Shared_Ptr<T>::operator*() const noexcept
{
  return *getPtr();
}

template<class T>
T* Shared_Ptr<T>::operator->() const noexcept
{
  return getPtr();
}

template<class T>
Shared_Ptr<T>::operator bool() const noexcept
{
  return (m_memBlock != nullptr);
}

template<class T>
bool Shared_Ptr<T>::valid() const noexcept
{
  return (m_memBlock != nullptr);
}

template<class T>
T* Shared_Ptr<T>::getPtr() noexcept
{
  return reinterpret_cast<T*>(m_memBlock + sizeof(Ref_Count));
}

template<class T>
T* Shared_Ptr<T>::getPtr() const noexcept
{
  return reinterpret_cast<T*>(m_memBlock + sizeof(Ref_Count));
}

template<class T>
Shared_Ptr<T>::Ref_Count_Pointer Shared_Ptr<T>::getRefCount() noexcept
{
  return reinterpret_cast<Ref_Count_Pointer>(m_memBlock);
}

template<class T>
byte* Shared_Ptr<T>::allocateMemBlock() noexcept
{
  return reinterpret_cast<byte*>(Memory_Manager::getDefaultAllocator()->allocate(sizeof(T) + sizeof(Ref_Count)));
}

template<class T>
void Shared_Ptr<T>::release() noexcept
{
  if (m_memBlock == nullptr)
    return;

  assert(*getRefCount() != 0);

  (*getRefCount())--;

  if (*getRefCount() == 0)
  {
    destructorAt(getPtr());
    Memory_Manager::getDefaultAllocator()->deallocate(m_memBlock);
    m_memBlock = nullptr;
  }
  return;
}

template<class U, typename... Args>
Shared_Ptr<U> makeShared(Args&&... args) noexcept
{
  Shared_Ptr<U> impl;

  impl.m_memBlock = impl.allocateMemBlock();
  constructAt(impl.getPtr(), std::forward<Args>(args)...);
  (*impl.getRefCount()) = 1;

  return impl;
}

} // namespace crude