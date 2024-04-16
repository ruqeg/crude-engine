#pragma once

#include <assert.h>

#define CRUDE_ASSERTIONS_ENABLED _DEBUG

#ifdef CRUDE_ASSERTIONS_ENABLED
#define CRUDE_ASSERT(expr) \
  if (expr) { } \
  else \
  { \
    assert(expr); \
  }
#else
#define CRUDE_ASSERT(expr)
#endif

#define CRUDE_STATIC_ASSERT(expr) \
  static_assert(expr, "static assert failed:" #expr)