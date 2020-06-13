#include <logger.hpp>

namespace hazel
{
    std::shared_ptr<spdlog::logger> Logger::dev_logger;

    std::shared_ptr<spdlog::logger> Logger::client_logger;

    void Logger::init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        Logger::dev_logger = spdlog::stdout_color_mt("hazel-dev");
        Logger::dev_logger->set_level(spdlog::level::trace);
        Logger::client_logger = spdlog::stdout_color_mt("application");
        Logger::client_logger->set_level(spdlog::level::trace);
    }
} // namespace hazel