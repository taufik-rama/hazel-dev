#pragma once

#include <hazel/core/logger.hpp>
#include <hazel/core/memory.hpp>
#include <hazel/trace/profiler.hpp>

#include <std-pch.hpp>

#define SHIFT(x) (1 << x)

#ifdef HAZEL_DEBUG
#define ASSERT(condition, message)                                             \
  do {                                                                         \
    if (!(condition)) {                                                        \
      std::cerr << "Assertion `" #condition "` failed in " << __FILE__         \
                << " line " << __LINE__ << ": " << message << std::endl;       \
      std::terminate();                                                        \
    }                                                                          \
  } while (false)
#else
#define ASSERT(condition, message)                                             \
  do {                                                                         \
  } while (false)
#endif

#define HAZEL_APPEND(x, y) x##y
#define HAZEL_APPEND_INDIRECT(x, y) HAZEL_APPEND(x, y)

// Declare `var` with `__LINE__` appended
#define HAZEL_VARLINE(var) HAZEL_APPEND_INDIRECT(var, __LINE__)
