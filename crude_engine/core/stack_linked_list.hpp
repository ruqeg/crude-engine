#pragma once

#include <core/alias.hpp>

namespace crude_engine
{

template <class T>
class Stack_Linked_List
{
protected:
  using Data_Type = T;
public:
  struct Node
  {
    Data_Type  m_data;
    Node* m_next;
  };
public:
  Stack_Linked_List() = default;
  Stack_Linked_List(const Stack_Linked_List& other) = delete;
  void push(Node* node) noexcept;
  Node* pop() noexcept;
protected:
  Node* m_head;
};

}

#include <core/stack_linked_list.inl>