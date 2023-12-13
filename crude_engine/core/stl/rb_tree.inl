#include <core/stl/rb_tree.hpp>

namespace crude_engine
{

template <class Node>
void RBT_Node_Base<Node>::setColor(RB_Color color) noexcept
{
  m_color = color;
}

template <class Node>
void RBT_Node_Base<Node>::makeBlack() noexcept
{
  m_color = RB_Color::BLACK;
}

template <class Node>
void RBT_Node_Base<Node>::makeRed() noexcept
{
  m_color = RB_Color::RED;
}

template <class Node>
RB_Color RBT_Node_Base<Node>::getColor() const noexcept
{
  return m_color;
}

template<class Node>
bool RBT_Node_Base<Node>::isBlack() const
{
  return (m_color == RB_Color::BLACK);
}

template<class Node>
bool RBT_Node_Base<Node>::isRed() const
{
  return (m_color == RB_Color::RED);
}

template <class Node>
void RBT_Node_Base<Node>::swapParentWith(Node* other) noexcept
{
  this->m_parent = other.m_parent;
}

template <class Node>
void RBT_Node_Base<Node>::swapColorWith(Node* other) noexcept
{
  m_color = other.m_color;
}

template<class Node, class Compare>
void Red_Black_Tree<Node, Compare>::insert(Node& node) noexcept
{
  this->m_size.add(1);

  Node* parent = this->m_root;
  Node* cur = this->m_root;

  while (cur != nullptr)
  {
    parent = cur;

    if constexpr (multiple)
    {
      if (this->m_cmp(*cur, node))
      {
        cur = cur->getRight();
      }
      else
      {
        cur = cur->getLeft();
      }
    }
    else
    {
      if (this->m_cmp(*cur, node))
      {
        cur = cur->getRight();
      }
      else if (this->m_cmp(node, *cur))
      {
        cur = cur->getLeft();
      }
      else
      {
        this->m_size.reduce(1);
        return;
      }
    }
  }

  if (parent == nullptr)
  {
    node.NB::setParent(nullptr);
    node.NB::makeBlack();
    this->m_root = &node;
    return;
  }

  node.NB::setParent(parent);
  node.NB::makeRed();

  if (this->m_cmp(node, *parent))
  {
    parent->NB::setLeft(&node);
  }
  else if (this->m_cmp(*parent, node))
  {
    parent->NB::setRight(&node);
  }
  else
  {
    if constexpr (!multiple)
    {
      this->m_size.reduce(1);
      return;
    }
    else
    {
      parent->NB::setLeft(&node);
    }
  }

  balanceAfterInsert(&node);
}

template<class Node, class Compare>
void Red_Black_Tree<Node, Compare>::balanceAfterInsert(Node* node)
{
  while
  (
    (node->NB::getParent()->NB::isRed()) // parend red
    &&
    (this->getUncle(node) != nullptr) && (this->getUncle(node)->NB::isRed()) // uncle red
  )
  {
    Node* parent = node->NB::getParent();
    parent->NB::makeBlack();
    this->getUncle(node)->NB::makeBlack();

    Node* grandparent = parent->NB::getParent();

    if (grandparent->NB::getParent() == nullptr)
    {
      return;
    }

    grandparent->NB::makeRed();

    node = grandparent;
  }

  Node* parent = node->NB::getParent();
  Node* grandparent = parent->NB::getParent();

  if (parent->NB::isBlack())
  {
    return;
  }

  if (grandparent->NB::getLeft() == parent)
  {
    if (parent->NB::getRight() == node)
    {
      rotateLeft(parent);
      node->NB::makeBlack();
    }
    else
    {
      parent->NB::makeBlack();
    }

    rotateRight(grandparent);
  }
  else
  {
    if (parent->NB::getLeft() == node)
    {
      rotateRight(parent);
      node->NB::makeBlack();
    }
    else
    {
      parent->NB::makeBlack();
    }
    rotateLeft(grandparent);
  }

  grandparent->NB::makeRed();
}

template<class Node, class Compare>
void Red_Black_Tree<Node, Compare>::rotateRight(Node* parent)
{
  Node* leftChild = parent->NB::getLeft();
  Node* grandparent = parent->NB::getParent();
  
  parent->NB::setLeft(leftChild->NB::getRight());

  if (leftChild->NB::getRight() != nullptr)
  {
    leftChild->NB::getRight()->NB::setParent(parent);
  }

  leftChild->NB::setParent(grandparent);
  leftChild->NB::setRight(parent);

  if (grandparent != nullptr)
  {
    if (grandparent->NB::getLeft() == parent)
    {
      grandparent->NB::setLeft(leftChild);
    }
    else
    {
      grandparent->NB::setRight(leftChild);
    }
  }
  else
  {
    this->m_root = leftChild;
  }

  parent->NB::setParent(leftChild);
}

template<class Node, class Compare>
void Red_Black_Tree<Node, Compare>::rotateLeft(Node* parent)
{
  Node* rightChild = parent->NB::getRight();
  Node* grandparent = parent->NB::getParent();
  
  parent->NB::setRight(rightChild->NB::getLeft());

  if (rightChild->NB::getLeft() != nullptr)
  {
    rightChild->NB::getLeft()->NB::setParent(parent);
  }

  rightChild->NB::setParent(grandparent);
  rightChild->NB::setLeft(parent);

  if (grandparent != nullptr)
  {
    if (grandparent->NB::getLeft() == parent)
    {
      grandparent->NB::setLeft(rightChild);
    }
    else
    {
      grandparent->NB::setRight(rightChild);
    }
  }
  else
  {
    this->m_root = rightChild;
  }

  parent->NB::setParent(rightChild);
}

template<class Node, class Compare>
void Red_Black_Tree<Node, Compare>::remove(Node& node) noexcept
{
}

template<class Node, class Compare>
Red_Black_Tree<Node, Compare>::Red_Black_Tree() noexcept
{}

template<class Node, class Compare>
Red_Black_Tree<Node, Compare>::Red_Black_Tree(RBT&& other) noexcept
  :
  BST(other)
{}

template<class Node, class Compare>
Red_Black_Tree<Node, Compare>& Red_Black_Tree<Node, Compare>::operator=(RBT&& other) noexcept
{
  BST::operator=(other);
}

}