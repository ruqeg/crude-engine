module;

export module crude.core.cxx_allocator_container;

import crude.core.iallocator;

export namespace crude::core
{

class CXX_Allocator_Container : public IAllocator
{
public:
  template<class T, typename... Args>
  [[nodiscard]] T* cxxAllocate(Args&&... args) noexcept;

  template<class T, typename... Args>
  [[nodiscard]] T* cxxAllocate(size_t n, Args&&... args) noexcept;

  template<class T>
  void cxxDeallocate(T* ptr) noexcept;

  template<class T>
  void cxxDeallocate(size_t n, T* ptr) noexcept;
};

}