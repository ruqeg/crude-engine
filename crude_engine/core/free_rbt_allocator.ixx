module;

export module crude_engine.free_rbt_allocator;

import crude_engine.cxx_allocator_container;
import crude_engine.rb_tree;
import crude_engine.alias;

export namespace crude_engine
{

class Free_RBT_Allocator : public CXX_Allocator_Container
{
public:
  enum Placement_Policy
  {
    PLACEMANT_POLICY_FIND_FIRST,
    PLACEMANT_POLICY_FIND_BEST
  };

  struct Node : public RBT_Node_Base<Node>
  {
    Node(size_t blockSize, bool64 free, Node* prev, Node* next) noexcept;
    Node*       next;
    Node*       prev;
    size_t      blockSize;
    bool64      free;
    inline bool operator<(const Node& other) const;
  };

public:
  // !TODO make for PLACEMANT_POLICY_FIND_BEST
  Free_RBT_Allocator(const size_t capacity, Placement_Policy placementPolicy = PLACEMANT_POLICY_FIND_BEST) noexcept;
  ~Free_RBT_Allocator() noexcept;

public:
  [[nodiscard]] void* allocate(size_t size) noexcept override; // O(log(n))
  void deallocate(void* ptr) noexcept override; // O(log(n))
  void reset() noexcept;

private:
  void assertLostMemoryBlock();
  void assertCorruptedMemoryList();
  void assertUnmergedMemoryBlocks();

private:
  byte*                 m_heap{ nullptr };
  size_t                m_heapSize;
  const size_t          m_capacity;
  Red_Black_Tree<Node>  m_rbt;
  Placement_Policy      m_policy;
};

}