module crude_engine.core.bs_tree;

namespace crude_engine
{

template<class Node>
BST_Node_Base<Node>::BST_Node_Base() noexcept
  :
  m_parent(nullptr)
{
  m_children[0] = nullptr;
  m_children[1] = nullptr;
}

template<class Node>
void BST_Node_Base<Node>::setParent(Node* newParent) noexcept
{
  m_parent = newParent;
}

template<class Node>
Node* BST_Node_Base<Node>::getParent() const noexcept
{
  return m_parent;
}

template<class Node>
void BST_Node_Base<Node>::setLeft(Node* newLeft) noexcept
{
  m_children[0] = newLeft;
}

template<class Node>
void BST_Node_Base<Node>::setRight(Node* newRight) noexcept
{
  m_children[1] = newRight;
}

template<class Node>
Node*& BST_Node_Base<Node>::getLeft() noexcept
{
  return m_children[0];
}

template<class Node>
Node*& BST_Node_Base<Node>::getRight() noexcept
{
  return m_children[1];
}

template<class Node>
Node* const& BST_Node_Base<Node>::getLeft() const noexcept
{
  return m_children[1];
}

template<class Node>
Node* const& BST_Node_Base<Node>::getRight() const noexcept
{
  return m_children[0];
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Binary_Search_Tree() noexcept
  :
  m_root(nullptr)
{}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Binary_Search_Tree(BST&& other) noexcept
  :
  m_root(other.m_root),
  m_size(other.m_size)
{
  other.m_root = nullptr;
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>& Binary_Search_Tree<Node, Compare>::operator=(BST&& other) noexcept
{
  m_root = other.m_root;
  m_size = other.m_size;
  other.m_root = nullptr;
}

template<class Node, class Compare>
template<class Comparable>
Binary_Search_Tree<Node, Compare>::Iterator Binary_Search_Tree<Node, Compare>::find(const Comparable& query) noexcept
{
  Node* cur = m_root;

  while (cur != nullptr)
  {
    if (m_cmp(*cur, query))
    {
      cur = cur->NB::getRight();
    }
    else if (m_cmp(query, *cur))
    {
      cur = cur->NB::getLeft();
    }
    else
    {
      return Iterator(cur);
    }
  }

  return end();
}

template<class Node, class Compare>
template<class Comparable>
Binary_Search_Tree<Node, Compare>::Const_Iterator Binary_Search_Tree<Node, Compare>::find(const Comparable& query) const noexcept
{
  return Const_Iterator(find(query));
}

template<class Node, class Compare>
Node* crude_engine::Binary_Search_Tree<Node, Compare>::getFirstEqual(Node* node) noexcept
{
  auto it = iteratorTo(*node);
  if (it == begin())
  {
    return node;
  }

  Node* ret = node;

  --it;
  while (!m_cmp(*it, *node))
  {
    ret = &(*it);
    --it;
  }

  return ret;
}

template<class Node, class Compare>
template<class Comparable>
Binary_Search_Tree<Node, Compare>::Iterator Binary_Search_Tree<Node, Compare>::upperBound(const Comparable& query) noexcept
{
  Node* cur = m_root;
  Node* lastLeft = nullptr;

  while (cur != nullptr)
  {
    if (m_cmp(query, *cur))
    {
      lastLeft = cur;
      cur = cur->NB::getLeft();
    }
    else
    {
      cur = cur->NB::getRight();
    }
  }

  if (lastLeft != nullptr)
  {
    return Iterator(lastLeft);
  }

  return end();
}

template<class Node, class Compare>
template<class Comparable>
Binary_Search_Tree<Node, Compare>::Iterator crude_engine::Binary_Search_Tree<Node, Compare>::lowerBound(const Comparable& query) noexcept
{
  Node* cur = m_root;
  Node* lastLeft = nullptr;

  while (cur != nullptr)
  {
    if (m_cmp(*cur, query))
    {
      cur = cur->NB::getRight();
    }
    else
    {
      lastLeft = cur;
      cur = cur->NB::getLeft();
    }
  }

  if (lastLeft != nullptr)
  {
    return Iterator(lastLeft);
  }

  return end();
}

template<class Node, class Compare>
template<class Comparable>
Binary_Search_Tree<Node, Compare>::Const_Iterator Binary_Search_Tree<Node, Compare>::lowerBound(const Comparable& query) const noexcept
{
  return Const_Iterator(const_cast<BST*>(this)->lowerBound(query));
}

template<class Node, class Compare>
template<class Comparable>
Binary_Search_Tree<Node, Compare>::Const_Iterator Binary_Search_Tree<Node, Compare>::upperBound(const Comparable& query) const noexcept
{
  return Const_Iterator(const_cast<BST*>(this)->upperBound(query));
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Const_Iterator Binary_Search_Tree<Node, Compare>::cbegin() const noexcept
{
  return Const_Iterator(getSmallest());
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Const_Iterator Binary_Search_Tree<Node, Compare>::cend() const noexcept
{
  return Const_Iterator(nullptr);
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Const_Iterator Binary_Search_Tree<Node, Compare>::begin() const noexcept
{
  return cbegin();
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Iterator Binary_Search_Tree<Node, Compare>::begin() noexcept
{
  return Iterator(getSmallest());
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Const_Iterator Binary_Search_Tree<Node, Compare>::end() const noexcept
{
  return cend();
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Iterator Binary_Search_Tree<Node, Compare>::end() noexcept
{
  return Iterator(nullptr);
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Const_Iterator Binary_Search_Tree<Node, Compare>::crbegin() const noexcept
{
  return Const_Iterator(getSmallest());
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Const_Iterator Binary_Search_Tree<Node, Compare>::crend() const noexcept
{
  return Const_Iterator(getLargest());
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Const_Iterator Binary_Search_Tree<Node, Compare>::rbegin() const noexcept
{
  return crbegin();
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Iterator Binary_Search_Tree<Node, Compare>::rbegin() noexcept
{
  return Iterator(getSmallest());
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Const_Iterator Binary_Search_Tree<Node, Compare>::rend() const noexcept
{
  return crend();
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Iterator Binary_Search_Tree<Node, Compare>::rend() noexcept
{
  return Iterator(getLargest());
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Const_Iterator Binary_Search_Tree<Node, Compare>::iteratorTo(const Node& node) const noexcept
{
  return Const_Iterator(&node);
}

template<class Node, class Compare>
Binary_Search_Tree<Node, Compare>::Iterator Binary_Search_Tree<Node, Compare>::iteratorTo(Node& node) noexcept
{
  return Iterator(&node);
}

template<class Node, class Compare>
size_t Binary_Search_Tree<Node, Compare>::size() const noexcept
{
  return m_size.get();
}

template<class Node, class Compare>
void Binary_Search_Tree<Node, Compare>::clear() noexcept
{
  m_root = nullptr;
  m_size.set(0u);
}

template<class Node, class Compare>
bool Binary_Search_Tree<Node, Compare>::empty() const noexcept
{
  return (m_root == nullptr);
}

template<class Node, class Compare>
Node* Binary_Search_Tree<Node, Compare>::getRoot() const noexcept
{
  return m_root;
}

template<class Node, class Compare>
Node* Binary_Search_Tree<Node, Compare>::getSmallest() const noexcept
{
  Node* smallest = m_root;

  if (smallest == nullptr)
  {
    return nullptr;
  }

  while (smallest->NB::getLeft() != nullptr)
  {
    smallest = smallest->NB::getLeft();
  }

  return smallest;
}

template<class Node, class Compare>
Node* Binary_Search_Tree<Node, Compare>::getLargest() const noexcept
{
  Node* largest = m_root;

  if (largest == nullptr)
  {
    return nullptr;
  }

  while (largest->NB::getRight() != nullptr)
  {
    largest = largest->NB::getRight();
  }

  return largest;
}

template<class Node, class Compare>
Node* Binary_Search_Tree<Node, Compare>::getUncle(Node* node) const noexcept
{
  Node* parent = node->NB::getParent();
  if (parent == nullptr)
  {
    return nullptr;
  }

  Node* grandparent = parent->NB::getParent();
  if (grandparent == nullptr)
  {
    return nullptr;
  }

  if (grandparent->NB::getLeft() == parent)
  {
    return grandparent->NB::getRight();
  }

  return grandparent->NB::getLeft();
}

} // namespace crude_engine