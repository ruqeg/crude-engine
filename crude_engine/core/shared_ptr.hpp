#pragma once

#include <core/memory_system.hpp>
#include <core/assert.hpp>
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
  Shared_Ptr(nullptr_t) noexcept;
  Shared_Ptr(const T& value) noexcept;
  template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
  Shared_Ptr(const Shared_Ptr<U, UAllocator>& other) noexcept;
  template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
  Shared_Ptr(Shared_Ptr<U, UAllocator>&& other) noexcept;
  Shared_Ptr(const Shared_Ptr<T, Allocator>& other) noexcept;
  Shared_Ptr(Shared_Ptr<T, Allocator>&& other) noexcept;

  template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
  CRUDE_INLINE Shared_Ptr<T, Allocator>& operator=(const Shared_Ptr<U, UAllocator>& other) noexcept;
  template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
  CRUDE_INLINE Shared_Ptr<T, Allocator>& operator=(Shared_Ptr<U, UAllocator>&& other) noexcept;
  CRUDE_INLINE Shared_Ptr<T, Allocator>& operator=(const Shared_Ptr<T, Allocator>& other) noexcept;
  CRUDE_INLINE Shared_Ptr<T, Allocator>& operator=(Shared_Ptr<T, Allocator>&& other) noexcept;

  template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
  CRUDE_INLINE bool operator==(const Shared_Ptr<U, UAllocator>& other) const noexcept;
  template<class U, class UAllocator> requires Same_As<Allocator, UAllocator>
  CRUDE_INLINE bool operator!=(const Shared_Ptr<U, UAllocator>& other) const noexcept;

  CRUDE_INLINE T& operator*() noexcept;
  CRUDE_INLINE T* operator->() noexcept;

  CRUDE_INLINE T& operator*() const noexcept;
  CRUDE_INLINE T* operator->() const noexcept;

  explicit operator bool() const noexcept;

protected:
  CRUDE_INLINE T* getPtr() noexcept;
  CRUDE_INLINE T* getPtr() const noexcept;
  CRUDE_INLINE Ref_Count_Pointer getRefCount() noexcept;
  CRUDE_INLINE CRUDE_NODISCARD byte* allocateMemBlock() noexcept;
  CRUDE_INLINE void release() noexcept;


protected:
  byte* m_memBlock;

  template<class U, class UAllocator, typename... Args>
  friend Shared_Ptr<U, UAllocator> makeShared(Args&&... args) noexcept;

  template<typename U, class UAllocator>
  friend class Shared_Ptr;
};

} // namespace crude_engine

#include <core/shared_ptr.inl>