#pragma once

#include <core/tree_iterator_base.hpp>
#include <core/size_holder.hpp>
#include <core/compare.hpp>

namespace crude_engine
{

template <class Node>
class BST_Node_Base
{
public:
  BST_Node_Base() noexcept;
  CRUDE_INLINE void setParent(Node* newParent) noexcept;
  CRUDE_INLINE Node* getParent() const noexcept;

  CRUDE_INLINE void setLeft(Node* newLeft) noexcept;
  CRUDE_INLINE void setRight(Node* newRight) noexcept;
  CRUDE_INLINE Node*& getLeft() noexcept;
  CRUDE_INLINE Node*& getRight() noexcept;
  CRUDE_INLINE Node* const& getLeft() const noexcept;
  CRUDE_INLINE Node* const& getRight() const noexcept;

protected:
  Node* m_parent;
  Node* m_children[2];
};

template <class Node, class Compare = Flexible_Less>
class Binary_Search_Tree
{
private:
  using BST = Binary_Search_Tree<Node, Compare>;
  using NB  = BST_Node_Base<Node>;

private:
  class Node_Interface
  {
  public:
    static CRUDE_INLINE Node* getParent(Node* node) noexcept
    {
      return node->NB::getParent();
    }

    static CRUDE_INLINE Node* getLeft(Node* node) noexcept
    {
      return node->NB::getLeft();
    }

    static CRUDE_INLINE Node* getRight(Node* node) noexcept
    {
      return node->NB::getRight();
    }

    static CRUDE_INLINE const Node* getParent(const Node* node) noexcept
    {
      return node->NB::getParent();
    }

    static CRUDE_INLINE const Node* getLeft(const Node* node) noexcept
    {
      return node->NB::getLeft();
    }

    static CRUDE_INLINE const Node* getRight(const Node* node) noexcept
    {
      return node->NB::getRight();
    }
  };

public:
  class Iterator : public Tree_Iterator_Base<Iterator, Node, Node_Interface>
  {
  public:
    using Tree_Iterator_Base<Iterator, Node, Node_Interface>::Tree_Iterator_Base;

    Iterator(const Iterator& other) noexcept
      : 
      Tree_Iterator_Base<Iterator, Node, Node_Interface>(other.m_node)
    {};
    Iterator() noexcept
      : 
      Tree_Iterator_Base<Iterator, Node, Node_Interface>() 
    {};

    Iterator& operator=(const Iterator&) noexcept = default;

  private:
    friend class Const_Iterator;
  };

  class Const_Iterator : public Tree_Iterator_Base<Const_Iterator, const Node, Node_Interface>
  {
  public:
    using Tree_Iterator_Base<Const_Iterator, const Node, Node_Interface>::Tree_Iterator_Base;
    Const_Iterator(const Const_Iterator& other) noexcept
      : 
      Tree_Iterator_Base<Const_Iterator, const Node, Node_Interface>(other.m_node) 
    {};
    Const_Iterator(const Iterator& other) noexcept
      :
      Tree_Iterator_Base<Const_Iterator, const Node, Node_Interface>(other.m_node) {};
    Const_Iterator() noexcept
      :
      Tree_Iterator_Base<Const_Iterator, const Node, Node_Interface>() 
    {};

    Const_Iterator& operator=(const Const_Iterator&) noexcept = default;
  };

protected:
  inline Node* getFirstEqual(Node* node) noexcept;

public:
  Binary_Search_Tree() noexcept;
  Binary_Search_Tree(BST&& other) noexcept;
  BST& operator=(BST&& other) noexcept;

  template <class Comparable>
  Const_Iterator find(const Comparable& query) const noexcept;
  
  template <class Comparable> 
  Iterator find(const Comparable& query) noexcept;

  template <class Comparable>
  Const_Iterator upperBound(const Comparable& query) const noexcept;

  template <class Comparable>
  Iterator upperBound(const Comparable& query) noexcept;

  template <class Comparable>
  Const_Iterator lowerBound(const Comparable& query) const noexcept;

  template <class Comparable>
  Iterator lowerBound(const Comparable& query) noexcept;

  Const_Iterator cbegin() const noexcept;
  Const_Iterator cend() const noexcept;

  Const_Iterator begin() const noexcept;
  Iterator begin() noexcept;

  Const_Iterator end() const noexcept;
  Iterator end() noexcept;

  Const_Iterator crbegin() const noexcept;
  Const_Iterator crend() const noexcept;

  Const_Iterator rbegin() const noexcept;
  Iterator rbegin() noexcept;

  Const_Iterator rend() const noexcept;
  Iterator rend() noexcept;

  Const_Iterator iteratorTo(const Node& node) const noexcept;
  Iterator iteratorTo(Node& node) noexcept;

  size_t size() const noexcept;

  void clear() noexcept;

  bool empty() const noexcept;

  Node* getRoot() const noexcept;

protected:
  Node* getSmallest() const noexcept;
  Node* getLargest() const noexcept;
  Node* getUncle(Node* node) const noexcept;

protected:
  Node*              m_root;
  Compare            m_cmp;
  Size_Holder<true>  m_size;
};

} // namespace crude_engine

#include "bs_tree.inl"