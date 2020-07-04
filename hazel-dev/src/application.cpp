#include <hazel/application.hpp>

// Temporary, GL stuff
#include <glad/glad.h>
#include <hazel/renderer/shader.hpp>

namespace hazel
{
    Application *Application::instance = nullptr;

    Application::Application()
    {
        assert(!Application::instance);
        Application::instance = this;

        this->window = std::unique_ptr<Window>(Window::create());
        this->window->set_event_callback(std::bind(&Application::event_callback, this, std::placeholders::_1));

        this->imgui = new hazel::layer::ImGui();
        this->add_layer_overlay(this->imgui);

        glGenVertexArrays(1, &this->vertex_array);
        glBindVertexArray(this->vertex_array);

        glGenBuffers(1, &this->vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);

        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f, // 0
            0.5f, -0.5f, 0.0f,  // 1
            0.0f, 0.5f, 0.0f,   // 2
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        glGenBuffers(1, &this->index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer);

        unsigned int indices[3] = {0, 1, 2};
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        std::string vertex_source = R"(
            #version 460 core
            layout(location = 0) in vec3 a_Position;
            out vec3 position;
            void main() {
                position = a_Position;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";
        std::string fragment_source = R"(
            #version 460 core
            layout(location = 0) out vec4 color;
            in vec3 position;
            void main() {
                color = vec4(position * 0.5 + 0.5, 1.0);
            }
        )";
        this->shader.reset(new hazel::renderer::Shader(vertex_source,  fragment_source));
    }

    Application::~Application() {}

    void Application::event_callback(hazel::event::Event &e)
    {
        hazel::event::EventDispatcher dispatcher(e);
        dispatcher.dispatch<hazel::event::WindowCloseEvent>(EVENT_BIND_METHOD_1(Application::window_close_event_callback));

        // Handle for each layers
        for (auto it = this->layers.end(); it != this->layers.begin();)
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
        this->layers.add(layer);
        layer->on_attach();
    }

    void Application::add_layer_overlay(hazel::layer::Layer *layer)
    {
        this->layers.add_overlay(layer);
        layer->on_attach();
    }

    void Application::run()
    {
        this->running = true;
        while (this->running)
        {
            glClearColor(0.2, 0.2, 0.2, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            this->shader->bind();

            glBindVertexArray(this->vertex_array);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

            for (auto layer : this->layers)
            {
                layer->on_update();
            }

            this->imgui->begin();
            for (auto layer : this->layers)
            {
                layer->on_imgui_render();
            }
            this->imgui->end();

            this->window->on_update();
        }
    }
} // namespace hazel
