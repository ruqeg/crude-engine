#include "../../crude_engine/core/memory/c_allocator.hpp"
#include "../../crude_engine/core/memory/linear_allocator.hpp"
#include "../../crude_engine/core/memory/pool_allocator.hpp"
#include "../../crude_engine/core/memory/stack_allocator.hpp"
#include "../../crude_engine/core/stl/rb_tree.hpp"

#include <iostream>
#include <string>
#include <assert.h>


class My_Node : public crude_engine::RBT_Node_Base<My_Node>
{
public:
  crude_engine::float64  key;
  std::string            value;
  // ...

  bool operator<(const My_Node& other) const
  {
    return key < other.key;
  }
};

bool operator<(const My_Node& lhs, int rhs) {
  return lhs.key < rhs;
}
bool operator<(int lhs, const My_Node& rhs) {
  return lhs < rhs.key;
}

int main()
{
  using RBT = crude_engine::Red_Black_Tree<My_Node>;
  
  RBT t;

  My_Node nodes[250];

  for (std::size_t i = 0; i < 250; ++i)
  {
    nodes[i].key = i;
    nodes[i].value = std::string("The key is ") + std::to_string(i);
  }

  // Insert them
  for (size_t i = 0; i < 250; ++i)
  {
    t.insert(nodes[i]);
  }

  auto it = t.find(3);
  assert(it != t.end());
  std::string retrieved_value = it->value;

  t.remove(*it);

  for (const auto& n : t)
  {
    std::cout << "A node: " << n.value << "\n";
  }

  return 1;
}