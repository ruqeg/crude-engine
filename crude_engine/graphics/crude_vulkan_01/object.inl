namespace crude_vulkan_01
{

template<class Type>
Type& TObject<Type>::getHandle()
{
  return m_handle;
}

template<class Type>
const Type& TObject<Type>::getHandle() const
{
  return m_handle;
}

} // namespace crude_vulkan_01
