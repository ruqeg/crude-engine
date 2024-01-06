#pragma once

namespace crude_engine
{

template<class Type>
class TObject
{
public:
  Type& getHandle();
  const Type& getHandle() const;
protected:
  Type m_handle;
};

#define CRUDE_OBJECT_HANDLE(pData) pData->getHandle()

} // namespace crude_engine

#include "object.inl"
