#include <hazel/application.hpp>

// Temporary, glClearColor
#include <GLFW/glfw3.h>

namespace hazel
{
    Application::Application()
    {
        this->window = std::unique_ptr<Window>(Window::Create());
        this->window->set_event_callback(std::bind(&Application::event_callback, this, std::placeholders::_1));
    }

    Application::~Application() {}

    void Application::event_callback(hazel::event::Event &e)
    {
        hazel::event::EventDispatcher dispatcher(e);
        dispatcher.dispatch<hazel::event::WindowCloseEvent>(std::bind(&Application::window_close_event_callback, this, std::placeholders::_1));
        HAZEL_DEV_LOG_TRACE("{}", e);
    }

    bool Application::window_close_event_callback(hazel::event::Event &)
    {
        this->running = false;
        return true;
    }

    void Application::Run()
    {
        this->running = true;
        while (this->running)
        {
            glClearColor(0.5, 0.5, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            this->window->on_update();
        }
    }
} // namespace hazel
