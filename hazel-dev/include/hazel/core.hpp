#pragma once

#include <hazel/memory.hpp>
#include <hazel/logger.hpp>

#include <std-pch.hpp>

#define SHIFT(x) (1 << x)

#ifdef HAZEL_DEBUG
#define ASSERT(condition, message)                                             \
    do                                                                         \
    {                                                                          \
        if (!(condition))                                                      \
        {                                                                      \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__   \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate();                                                  \
        }                                                                      \
    } while (false)
#else
#define ASSERT(condition, message) \
    do                             \
    {                              \
    } while (false)
#endif
