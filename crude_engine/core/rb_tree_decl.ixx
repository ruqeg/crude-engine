module;

export module crude_engine.core.rb_tree.decl;

import crude_engine.core.bs_tree;
import crude_engine.core.alias;
import crude_engine.core.compare;
import crude_engine.core.utility;

export namespace crude_engine
{

enum class RB_Color : uint8
{
  BLACK = 0,
  RED = 1
};

template <class Node>
class RBT_Node_Base : public BST_Node_Base<Node>
{
public:
  RBT_Node_Base() noexcept;
  void setColor(RB_Color color) noexcept;
  void makeBlack() noexcept;
  void makeRed() noexcept;
  RB_Color getColor() const noexcept;
  bool isBlack() const;
  bool isRed() const;

  void swapParentWith(Node* other) noexcept;
  void swapColorWith(Node* other) noexcept;

protected:
  RB_Color  m_color;
};

template <class Node, class Compare = Flexible_Less>
class Red_Black_Tree : public Binary_Search_Tree<Node, Compare>
{
public:
  using RBT = Red_Black_Tree<Node, Compare>;
  using NB = RBT_Node_Base<Node>;
  using BST = Binary_Search_Tree<Node, Compare>;

  using Iterator = BST::Iterator;
  using Const_Iterator = BST::Const_Iterator;

public:
  Red_Black_Tree() noexcept;
  Red_Black_Tree(RBT&& other) noexcept;
  RBT& operator=(RBT&& other) noexcept;

  void insert(Node& node) noexcept;
  void remove(Node& node) noexcept;

protected:
  void balanceAfterInsert(Node* node) noexcept;
  void rotateRight(Node* parent) noexcept;
  void rotateLeft(Node* parent) noexcept;
  void swapNodes(Node* n1, Node* n2, bool swapColors) noexcept;
  void swapUnrelatedNodes(Node* n1, Node* n2) noexcept;
  void swapNeighbors(Node* parent, Node* child) noexcept;

protected:
  static constexpr bool multiple = true;
};

} // namespace crude_engine