#pragma once

#include <spdlog-pch.hpp>

namespace hazel
{
    class Logger
    {
    public:
        static void init();

        static std::shared_ptr<spdlog::logger> &get_dev_logger() { return dev_logger; }

        static std::shared_ptr<spdlog::logger> &get_client_logger() { return client_logger; }

    private:
        static std::shared_ptr<spdlog::logger> dev_logger;

        static std::shared_ptr<spdlog::logger> client_logger;
    };

} // namespace hazel

#define FORMAT_TO_STRING(type)                             \
    template <typename OStream>                            \
    friend OStream &operator<<(OStream &os, const type &e) \
    {                                                      \
        return os << e.to_string();                        \
    }

#define HAZEL_LOG_FORMAT(fmt) "[{} {}:{}] " fmt

#ifdef HAZEL_DEBUG
#define HAZEL_DEV_LOG_TRACE(fmt, ...)         \
    ::hazel::Logger::get_dev_logger()->trace( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_DEV_LOG_INFO(fmt, ...)         \
    ::hazel::Logger::get_dev_logger()->info( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_DEV_LOG_WARN(fmt, ...)         \
    ::hazel::Logger::get_dev_logger()->warn( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_DEV_LOG_ERROR(fmt, ...)         \
    ::hazel::Logger::get_dev_logger()->error( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_DEV_LOG_FATAL(fmt, ...)         \
    ::hazel::Logger::get_dev_logger()->fatal( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

#define HAZEL_CLIENT_LOG_TRACE(fmt, ...)         \
    ::hazel::Logger::get_client_logger()->trace( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_CLIENT_LOG_INFO(fmt, ...)         \
    ::hazel::Logger::get_client_logger()->info( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_CLIENT_LOG_WARN(fmt, ...)         \
    ::hazel::Logger::get_client_logger()->warn( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_CLIENT_LOG_ERROR(fmt, ...)         \
    ::hazel::Logger::get_client_logger()->error( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_CLIENT_LOG_FATAL(fmt, ...)         \
    ::hazel::Logger::get_client_logger()->fatal( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#else
#define HAZEL_DEV_LOG_TRACE(fmt, ...)
#define HAZEL_DEV_LOG_INFO(fmt, ...)
#define HAZEL_DEV_LOG_WARN(fmt, ...)
#define HAZEL_DEV_LOG_ERROR(fmt, ...)
#define HAZEL_DEV_LOG_FATAL(fmt, ...)

#define HAZEL_CLIENT_LOG_TRACE(fmt, ...)
#define HAZEL_CLIENT_LOG_INFO(fmt, ...)
#define HAZEL_CLIENT_LOG_WARN(fmt, ...)
#define HAZEL_CLIENT_LOG_ERROR(fmt, ...)
#define HAZEL_CLIENT_LOG_FATAL(fmt, ...)
#endif