#pragma once

#include <hazel/event/key.hpp>
#include <hazel/event/mouse.hpp>
#include <hazel/event/window.hpp>
#include <hazel/core.hpp>
#include <hazel/window.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace hazel::platform::linux
{
    class Window : public hazel::Window
    {
    public:
        Window(const hazel::WindowProps &);

        virtual ~Window();

        void on_update() override;

        virtual unsigned short get_width() const override;

        virtual unsigned short get_height() const override;

        virtual void set_event_callback(const EventCallbackFn &) override;

        virtual void set_vsync(bool) override;

        virtual bool get_vsync() const override;

    private:
        GLFWwindow *window;

        struct WindowData
        {
            std::string title;
            unsigned short width, height;
            bool vsync;
            hazel::Window::EventCallbackFn event_callback;
        } window_data;

        virtual void init(const hazel::WindowProps &);

        virtual void shutdown();
    };
} // namespace hazel::platform::linux
