#pragma once

#include <hazel/input/input.hpp>
#include <hazel/event/window.hpp>
#include <hazel/layer/_fwd.hpp>
#include <hazel/layer/collection.hpp>
#include <hazel/layer/imgui.hpp>
#include <hazel/layer/layer.hpp>
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
        hazel::layer::Collection layers;
        std::unique_ptr<Window> window;
        hazel::layer::ImGui *imgui;
        bool running;
        bool window_close_event_callback(hazel::event::Event &);
        static Application *instance;

        unsigned int vertex_array, vertex_buffer, index_buffer;
    };

    Application *create_application();

} // namespace hazel
