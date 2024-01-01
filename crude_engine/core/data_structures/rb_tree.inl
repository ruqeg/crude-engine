#include <core/data_structures/rb_tree.hpp>
#include <core/utility.hpp>

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
  this->m_parent = other->m_parent;
}

template <class Node>
void RBT_Node_Base<Node>::swapColorWith(Node* other) noexcept
{
  m_color = other->m_color;
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
void Red_Black_Tree<Node, Compare>::balanceAfterInsert(Node* node) noexcept
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
void Red_Black_Tree<Node, Compare>::rotateRight(Node* parent) noexcept
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
void Red_Black_Tree<Node, Compare>::rotateLeft(Node* parent) noexcept
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
  this->m_size.reduce(1u);

  Node* cur = &node;
  Node* child = &node;

  if ((cur->getRight() != nullptr) && (cur->getLeft() != nullptr))
  {
    child = cur->NB::getRight();
    while (child->NB::getLeft() != nullptr)
    {
      child = child->getLeft();
    }
  }
  else if (cur->getLeft() != nullptr)
  {
    child = child->getLeft();
  }

  if (child != &node)
  {
    swapNodes(&node, child, false);
  }

  // Tree with child
  if (node.getRight() != nullptr)
  {
    Node* rightChild = node.getRight();
    swapNodes(&node, rightChild, true);

    rightChild->makeBlack();
    rightChild->setRight(nullptr);
    return;
  }

  if (node.getParent() == nullptr)
  {
    this->m_root = nullptr;
    return;
  }

  bool bLeftChild = (node.getParent()->getLeft() == &node);
  if (bLeftChild)
  {
    node.getParent()->setLeft(nullptr);
  }
  else
  {
    node.getParent()->setRight(nullptr);
  }

  // Red tree without children
  if (node.isRed())
  {
    return;
  }

  Node* parent = node.getParent();

  // Black tree without children
  bool bPropagatingUp = true;
  Node* sibling = nullptr;

  while (bPropagatingUp)
  {
    if (bLeftChild)
    {
      sibling = parent->NB::getRight();
    }
    else
    {
      sibling = parent->NB::getLeft();
    }

    // 2.2.2.2
    if
    (
      (parent->isBlack())
      &&
      (sibling->isBlack())
      &&
      ((sibling->getLeft() == nullptr) || sibling->getLeft()->isBlack())
      &&
      ((sibling->getRight() == nullptr) || sibling->getRight()->isBlack())
    )
    {
      sibling->makeRed();
      if (parent->getParent() == nullptr)
      {
        return;
      }
      else
      {
        bLeftChild = (parent->getParent()->getLeft() == parent);
        parent = parent->getParent();
      }
    }
    else
    {
      bPropagatingUp = false;
    }
  }

  if (sibling->isRed())
  {
    sibling->makeBlack();
    parent->makeRed();
    if (bLeftChild)
    {
      rotateLeft(parent);
      sibling = parent->getRight();
    }
    else
    {
      rotateRight(parent);
      sibling = parent->getLeft();
    }
  }

  // 2.1.2
  if
  (
    sibling->isBlack()
    &&
    ((sibling->getLeft() == nullptr) || sibling->getLeft()->isBlack())
    &&
    ((sibling->getRight() == nullptr) || sibling->getRight()->isBlack())
  )
  {
    parent->makeBlack();
    sibling->makeRed();
    return;
  }

  if (bLeftChild)
  {
    if ((sibling->getRight() == nullptr) || sibling->getRight()->isBlack())
    {
      rotateRight(sibling);
      sibling->makeRed();
      sibling = sibling->getParent();
      sibling->makeBlack();
    }

    rotateLeft(parent);

    parent->swapColorWith(sibling);

    sibling->getRight()->makeBlack();
  }
  else
  {
    if ((sibling->getLeft() == nullptr) || sibling->getLeft()->isBlack())
    {
      rotateLeft(sibling);
      sibling->makeRed();
      sibling = sibling->getParent();
      sibling->makeBlack();
    }

    rotateRight(parent);
    parent->swapColorWith(sibling);
    sibling->getLeft()->makeBlack();
  }
}

template<class Node, class Compare>
void Red_Black_Tree<Node, Compare>::swapNodes(Node* n1, Node* n2, bool swapColors) noexcept
{
  if (n1->getParent() == n2)
  {
    swapNeighbors(n2, n1);
  }
  else if (n2->getParent() == n1)
  {
    swapNeighbors(n1, n2);
  }
  else
  {
    swapUnrelatedNodes(n1, n2);
  }

  if (!swapColors)
  {
    n1->swapColorWith(n2);
  }
}

template<class Node, class Compare>
void Red_Black_Tree<Node, Compare>::swapUnrelatedNodes(Node* n1, Node* n2) noexcept
{
  Utility::swap(n1->getLeft(), n2->getLeft());
  if (n1->getLeft() != nullptr)
  {
    n1->getLeft()->setParent(n1);
  }
  if (n2->getLeft() != nullptr)
  {
    n2->getLeft()->setParent(n2);
  }

  Utility::swap(n1->getRight(), n2->getRight());
  if (n1->getRight() != nullptr)
  {
    n1->getRight()->setParent(n1);
  }
  if (n2->getRight() != nullptr)
  {
    n2->getRight()->setParent(n2);
  }

  n1->swapParentWith(n2);

  if (n1->getParent() != nullptr)
  {
    if (n1->getParent()->getRight() == n2)
    {
      n1->getParent()->setRight(n1);
    }
    else
    {
      n1->getParent()->setLeft(n1);
    }
  }
  else
  {
    this->m_root = n1;
  }

  if (n2->getParent() != nullptr)
  {
    if (n2->getParent()->getRight() == n1)
    {
      n2->getParent()->setRight(n2);
    }
    else
    {
      n2->getParent()->setLeft(n2);
    }
  }
  else
  {
    this->m_root = n2;
  }
}

template<class Node, class Compare>
void Red_Black_Tree<Node, Compare>::swapNeighbors(Node* parent, Node* child) noexcept
{
  child->setParent(parent->getParent());
  parent->setParent(child);
  if (child->getParent() != nullptr)
  {
    if (child->getParent()->getLeft() == parent)
    {
      child->getParent()->setLeft(child);
    }
    else
    {
      child->getParent()->setRight(child);
    }
  }
  else
  {
    this->m_root = child;
  }

  if (parent->getLeft() == child)
  {
    parent->setLeft(child->getLeft());
    if (parent->getLeft() != nullptr)
    {
      parent->getLeft()->setParent(parent);
    }
    child->setLeft(parent);

    Utility::swap(parent->getRight(), child->getRight());
    if (child->getRight() != nullptr)
    {
      child->getRight()->setParent(child);
    }
    if (parent->getRight() != nullptr)
    {
      parent->getRight()->setParent(parent);
    }
  }
  else
  {
    parent->setRight(child->getRight());
    if (parent->getRight() != nullptr)
    {
      parent->getRight()->setParent(parent);
    }
    child->setRight(parent);

    Utility::swap(parent->getLeft(), child->getLeft());
    if (child->getLeft() != nullptr)
    {
      child->getLeft()->setParent(child);
    }
    if (parent->getLeft() != nullptr)
    {
      parent->getLeft()->setParent(parent);
    }
  }
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