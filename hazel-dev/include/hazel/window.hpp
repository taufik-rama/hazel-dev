#pragma once

#include <hazel/core.hpp>
#include <hazel/event/event.hpp>

namespace hazel
{
    class WindowProps
    {
    public:
        std::string title;
        unsigned short width;
        unsigned short height;

        WindowProps(
            const std::string &title = "Hazel Dev Engine",
            unsigned short width = 1280,
            unsigned short height = 720) : title(title),
                                           width(width),
                                           height(height) {}
    };

    class Window
    {
    public:
        using EventCallbackFn = std::function<void(event::Event &)>;

        virtual ~Window() {}

        virtual void on_update() = 0;

        virtual unsigned short get_width() const = 0;

        virtual unsigned short get_height() const = 0;

        virtual void set_event_callback(const EventCallbackFn &) = 0;

        virtual void set_vsync(bool) = 0;

        virtual bool get_vsync() const = 0;

        static Window *Create(const WindowProps &props = WindowProps());
    };
} // namespace hazel