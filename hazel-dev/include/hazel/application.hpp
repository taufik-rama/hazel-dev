#pragma once

#include <hazel/core.hpp>
#include <hazel/window.hpp>

#include <hazel/event/window.hpp>

namespace hazel
{
    class Application
    {
    public:
        Application();

        virtual ~Application();

        void Run();

        void event_callback(hazel::event::Event &);

    private:
        std::unique_ptr<Window> window;

        bool running;

        bool window_close_event_callback(hazel::event::Event &);
    };

    Application *create_application();

} // namespace hazel
