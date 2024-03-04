#pragma once

#include <core/data_structures/size_holder.hpp>
#include <core/data_structures/list_iterator_base.hpp>
#include <core/algorithms.hpp>
#include <core/memory/memory_system.hpp>

namespace crude_engine
{

template <class Node>
class List_Node_Base
{
public:
  List_Node_Base() noexcept;
  CRUDE_INLINE void setNext(Node* newNext) noexcept;
  CRUDE_INLINE void setPrev(Node* newPrev) noexcept;
  CRUDE_INLINE Node*& getNext() const noexcept;
  CRUDE_INLINE Node*& getPrev() const noexcept;

protected:
  Node* m_next;
  Node* m_prev;
};

template<class Node, class Compare = Flexible_Less>
class List
{
public:
  using Value_Type       = Node;
  using Size_Type        = std::size_t;
  using Difference_Type  = std::ptrdiff_t;
  using Reference        = Value_Type&;
  using Const_Reference  = const Value_Type&;
  using Pointer          = Value_Type*;
  using Const_Pointer    = const Value_Type*;

public:
  class Iterator : public List_Iterator_Base<Iterator, Value_Type>
  {
  public:
    using List_Iterator_Base<Iterator, Value_Type>::List_Iterator_Base;

    Iterator(const Iterator& other) noexcept
      :
      List_Iterator_Base<Iterator, Value_Type>(other.m_ptr)
    {};
    Iterator() noexcept
      :
      List_Iterator_Base<Iterator, Value_Type>()
    {};

    Iterator& operator=(const Iterator&) noexcept = default;

  private:
    friend class Const_Iterator;
  };

  class Const_Iterator : public List_Iterator_Base<Const_Iterator, const Value_Type>
  {
  public:
    using List_Iterator_Base<Const_Iterator, const Value_Type>::List_Iterator_Base;
    Const_Iterator(const Const_Iterator& other) noexcept
      :
      List_Iterator_Base<Const_Iterator, const Value_Type>(other.m_ptr)
    {};
    Const_Iterator(const Iterator& other) noexcept
      :
      List_Iterator_Base<Const_Iterator, const Value_Type>(other.m_ptr) {};
    Const_Iterator() noexcept
      :
      List_Iterator_Base<Const_Iterator, const Value_Type>()
    {};

    Const_Iterator& operator=(const Const_Iterator&) noexcept = default;
  };

public:
  List() noexcept;
  List(List&& other) noexcept;
  List& operator=(List&& other) noexcept;

  // !TODO
  //template <class Comparable>
  //Const_Iterator find(const Comparable& query) const noexcept;
  //
  //template <class Comparable>
  //Iterator find(const Comparable& query) noexcept;

  //Const_Iterator cbegin() const noexcept;
  //Const_Iterator cend() const noexcept;
  //
  //Const_Iterator begin() const noexcept;
  //Iterator begin() noexcept;
  //
  //Const_Iterator end() const noexcept;
  //Iterator end() noexcept;
  //
  //Const_Iterator crbegin() const noexcept;
  //Const_Iterator crend() const noexcept;
  //
  //Const_Iterator rbegin() const noexcept;
  //Iterator rbegin() noexcept;
  //
  //Const_Iterator rend() const noexcept;
  //Iterator rend() noexcept;
  //
  //void pushBack(Node*& node);
  //void pushFront(Node*& node);
  //
  //Node* popBack();
  //Node* popFront();
  //
  //size_t size() const noexcept;
  //
  //void clear() noexcept;
  //
  //bool empty() const noexcept;

protected:
  Node*              m_first;
  Node*              m_last;
  Compare            m_cmp;
  Size_Holder<true>  m_size;
};

} // namespace crude_engine

#include <core/data_structures/list.inl>