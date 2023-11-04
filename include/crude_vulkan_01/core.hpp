#pragma once

#include "types.hpp"
#include <stdexcept>
#include <memory>

namespace crude_vulkan_01 
{

class Core
{
public:
  static void copyArray(int8* dst, const int8* src, uint32 n);
};

// !TODO
#define CRUDE_VULKAN_01_HANDLE_RESULT(vkResult, error)
#define CRUDE_VULKAN_01_ERROR(msg)
#define CRUDE_VULKAN_01_HANDLE(pData) pData->getHandle()

#define CRUDE_VULKAN_01_NEW new
#define CRUDE_VULKAN_01_DELETE delete
#define CRUDE_VULKAN_01_COPY_MEMORY(dst, src, n) Core::copyArray(reinterpret_cast<int8*>(dst), reinterpret_cast<const int8*>(src), n)

}
