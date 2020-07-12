#include <hazel/application.hpp>

#include <hazel/camera/orthographic.hpp>
#include <hazel/event/window.hpp>
#include <hazel/layer/collection.hpp>
#include <hazel/layer/imgui.hpp>
#include <hazel/renderer/buffer.hpp>
#include <hazel/renderer/renderer.hpp>
#include <hazel/renderer/shader.hpp>
#include <hazel/renderer/vertex_array.hpp>

namespace hazel
{
    Application *Application::instance = nullptr;

    Application::Application()
    {
        assert(!Application::instance);
        Application::instance = this;

        this->layers = new hazel::layer::Collection();

        this->window = std::unique_ptr<Window>(Window::create());
        this->window->set_event_callback(std::bind(&Application::event_callback, this, std::placeholders::_1));

        this->imgui = new hazel::layer::ImGui();
        this->add_layer_overlay(this->imgui);
    }

    Application::~Application() {}

    void Application::event_callback(hazel::event::Event &e)
    {
        hazel::event::EventDispatcher dispatcher(e);
        dispatcher.dispatch<hazel::event::WindowCloseEvent>(EVENT_BIND_METHOD_1(Application::window_close_event_callback));

        // Handle for each layers
        for (auto it = this->layers->end(); it != this->layers->begin();)
        {
            --it;
            (*it)->on_event(e);
            if (e.is_handled())
            {
                break;
            }
        }
    }

    bool Application::window_close_event_callback(hazel::event::Event &)
    {
        this->running = false;
        return true;
    }

    void Application::add_layer(hazel::layer::Layer *layer)
    {
        this->layers->add(layer);
        layer->on_attach();
    }

    void Application::add_layer_overlay(hazel::layer::Layer *layer)
    {
        this->layers->add_overlay(layer);
        layer->on_attach();
    }

    void Application::run()
    {
        this->running = true;
        while (this->running)
        {
            for (auto layer : *this->layers)
            {
                layer->on_update();
            }

            this->imgui->begin();
            for (auto layer : *this->layers)
            {
                layer->on_imgui_render();
            }
            this->imgui->end();

            this->window->on_update();
        }
    }
} // namespace hazel
