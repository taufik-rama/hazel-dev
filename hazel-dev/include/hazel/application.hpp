#pragma once

#include <hazel/camera/_fwd.hpp>
#include <hazel/layer/_fwd.hpp>
#include <hazel/renderer/_fwd.hpp>
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

        std::shared_ptr<hazel::renderer::Shader> shader;
        std::shared_ptr<hazel::renderer::VertexArray> vertex_array;

        std::shared_ptr<hazel::renderer::Shader> square_shader;
        std::shared_ptr<hazel::renderer::VertexArray> square_vertex_array;

        std::shared_ptr<hazel::camera::Orthographic> camera;
    };

    Application *create_application();

} // namespace hazel
