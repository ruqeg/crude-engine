#include "../../crude_engine/core/memory/cxx_allocator.hpp"
#include "../../crude_engine/core/memory/linear_allocator.hpp"
#include "../../crude_engine/core/memory/pool_allocator.hpp"
#include "../../crude_engine/core/memory/stack_allocator.hpp"
#include "../../crude_engine/core/memory/free_rbt_allocator.hpp"
#include "../../crude_engine/core/data_structures/rb_tree.hpp"
#include "../../crude_engine/core/data_structures/array_stack.hpp"
#include "../../crude_engine/core/data_structures/array_dynamic.hpp"
#include "../../crude_engine/core/data_structures/shared_ptr.hpp"

#include <iostream>
#include <string>
#include <assert.h>

int main()
{
  constexpr std::size_t capacity = 10000000u;
  constexpr std::size_t elementsNum = 3u;
  crude_engine::Free_RBT_Allocator allocator(capacity, crude_engine::Free_RBT_Allocator::PLACEMANT_POLICY_FIND_BEST);
  int* arr[elementsNum];
  for (int i = 0; i < elementsNum; ++i)
  {
    arr[i] = (int*)allocator.allocate(sizeof(int));
    *arr[i] = i;
  }
  for (int i = 0; i < elementsNum; ++i)
  {
    std::cout << "element " << i << ": " << *arr[i] << std::endl;
  }
  for (int i = 0; i < elementsNum; ++i)
  {
    allocator.free(arr[i]);
  }

  return 1;
}