#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace hazel
{
    class logger
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

#define HAZEL_LOG_FORMAT(fmt) "[{} {}:{}] " fmt

#define HAZEL_DEV_LOG_TRACE(fmt, ...)         \
    ::hazel::logger::get_dev_logger()->trace( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_DEV_LOG_INFO(fmt, ...)         \
    ::hazel::logger::get_dev_logger()->info( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_DEV_LOG_WARN(fmt, ...)         \
    ::hazel::logger::get_dev_logger()->warn( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_DEV_LOG_ERROR(fmt, ...)         \
    ::hazel::logger::get_dev_logger()->error( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_DEV_LOG_FATAL(fmt, ...)         \
    ::hazel::logger::get_dev_logger()->fatal( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

#define HAZEL_CLIENT_LOG_TRACE(fmt, ...)         \
    ::hazel::logger::get_client_logger()->trace( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_CLIENT_LOG_INFO(fmt, ...)         \
    ::hazel::logger::get_client_logger()->info( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_CLIENT_LOG_WARN(fmt, ...)         \
    ::hazel::logger::get_client_logger()->warn( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_CLIENT_LOG_ERROR(fmt, ...)         \
    ::hazel::logger::get_client_logger()->error( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define HAZEL_CLIENT_LOG_FATAL(fmt, ...)         \
    ::hazel::logger::get_client_logger()->fatal( \
        HAZEL_LOG_FORMAT(fmt), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
