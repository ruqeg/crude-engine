module;

export module crude_engine.core.stack_linked_list;

import crude_engine.core.alias;

export namespace crude_engine
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

template<class T>
inline void Stack_Linked_List<T>::push(Node* node) noexcept
{
  node->m_next = m_head;
  m_head = node;
}

template<class T>
inline Stack_Linked_List<T>::Node* Stack_Linked_List<T>::pop() noexcept
{
  Node* top = m_head;
  m_head = m_head->m_next;
  return top;
}

}