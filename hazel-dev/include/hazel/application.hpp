#pragma once

#include <hazel/layer/_fwd.hpp>
#include <hazel/core.hpp>
#include <hazel/window.hpp>

namespace hazel
{
    class Application
    {
    public:
        Application();

        virtual ~Application();

        void run();

        void event_callback(hazel::event::Event &);

        void add_layer(hazel::layer::Layer *);

        void add_layer_overlay(hazel::layer::Layer *);

        static Application *get_application() { return instance; }

        Window &get_window() { return *this->window; };

    private:
        bool window_close_event_callback(hazel::event::Event &);

        hazel::layer::Collection *layers;
        std::unique_ptr<Window> window;
        hazel::layer::ImGui *imgui;

        static Application *instance;
        bool running;
    };

    Application *create_application();

} // namespace hazel
