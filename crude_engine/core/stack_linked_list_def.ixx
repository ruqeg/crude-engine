module;

export module crude_engine.core.stack_linked_list.def;

export import crude_engine.core.stack_linked_list.decl;

export namespace crude_engine
{

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