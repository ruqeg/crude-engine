module;

export module crude_engine.core.shared_ptr.decl;

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
  using Ref_Count = uint32;
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