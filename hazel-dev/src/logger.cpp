#include <logger.hpp>

namespace hazel
{
    std::shared_ptr<spdlog::logger> logger::dev_logger;

    std::shared_ptr<spdlog::logger> logger::client_logger;

    void logger::init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        logger::dev_logger = spdlog::stdout_color_mt("hazel-dev");
        logger::dev_logger->set_level(spdlog::level::trace);
        logger::client_logger = spdlog::stdout_color_mt("application");
        logger::client_logger->set_level(spdlog::level::trace);
    }
} // namespace hazel