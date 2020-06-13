#include <hazel/application.hpp>

namespace hazel
{
    Application::Application() {}

    Application::~Application() {}

    void Application::Run()
    {
        hazel::event::WindowResizeEvent e(1280, 720);
        HAZEL_DEV_LOG_TRACE("resize event: {}", e);

        while (true)
        {
        }
    }
} // namespace hazel
