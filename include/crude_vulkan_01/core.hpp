#pragma once

#include "types.hpp"
#include <stdexcept>
#include <memory>
#include <assert.h>
#include <vulkan/vk_enum_string_helper.h>

namespace crude_vulkan_01 
{

class Core
{
public:
  static void copyArray(int8* dst, const int8* src, uint32 n);
  static int8* initCopyArray(const int8* src, uint32 n);
  static char8* initCopyString(const char8* src);
};

// !TODO
#define CRUDE_VULKAN_01_ASSERT assert

#define CRUDE_VULKAN_01_HANDLE_RESULT(vkResult, error)
#define CRUDE_VULKAN_01_ERROR(msg)
#define CRUDE_VULKAN_01_HANDLE(pData) pData->getHandle()

#define CRUDE_VULKAN_01_NEW                                     new
#define CRUDE_VULKAN_01_DELETE                                  delete
#define CRUDE_VULKAN_01_COPY_MEMORY(T, dst, src, n)             Core::copyArray(reinterpret_cast<int8*>(dst), reinterpret_cast<const int8*>(src), sizeof(T) * (n))
#define CRUDE_VULKAN_01_NEW_COPY_MEMORY(T, src, n)              reinterpret_cast<T*>(Core::initCopyArray(reinterpret_cast<const int8*>(src), sizeof(T) * (n)))
#define CRUDE_VULKAN_01_NEW_COPY_STRING(src)                    Core::initCopyString(src)
#define CRUDE_VULKAN_01_OPTIONAL_REFVALUE_OR_NULL(optionalval)  ((optionalval).has_value() ? (&(optionalval).value()) : nullptr)

}
