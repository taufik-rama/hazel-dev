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

        // 16:9 aspect ratio
        this->camera.reset(new hazel::camera::Orthographic(-1.6f, 1.6f, -0.9f, 0.9f));

        this->layers = new hazel::layer::Collection();

        this->window = std::unique_ptr<Window>(Window::create());
        this->window->set_event_callback(std::bind(&Application::event_callback, this, std::placeholders::_1));

        this->imgui = new hazel::layer::ImGui();
        this->add_layer_overlay(this->imgui);

        this->vertex_array.reset(hazel::renderer::VertexArray::create());

        float vertices[7 * 3] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, // 0
            0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f,  // 1
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,   // 2
        };
        std::shared_ptr<hazel::renderer::VertexBuffer> vertex_buffer;
        vertex_buffer.reset(hazel::renderer::VertexBuffer::create(vertices, sizeof(vertices)));
        {
            hazel::renderer::BufferLayout layout = {
                {hazel::renderer::ShaderDataType::FLOAT3, "a_Position"},
                {hazel::renderer::ShaderDataType::FLOAT4, "a_Color"},
            };
            vertex_buffer->set_layout(layout);
        }
        this->vertex_array->add_vertex_buffer(vertex_buffer);

        const unsigned int indices_len = 3;
        unsigned int indices[indices_len] = {0, 1, 2};
        std::shared_ptr<hazel::renderer::IndexBuffer> index_buffer;
        index_buffer.reset(hazel::renderer::IndexBuffer::create(indices, indices_len));
        this->vertex_array->set_index_buffer(index_buffer);

        {
            std::string vertex_source = R"(
            #version 450 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            
            out vec3 v_Position;
            out vec4 v_Color;

            void main() {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";
            std::string fragment_source = R"(
            #version 450 core
            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main() {
                // color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";
            this->shader.reset(new hazel::renderer::Shader(vertex_source, fragment_source));
        }

        {
            this->square_vertex_array.reset(hazel::renderer::VertexArray::create());

            float vertices[4 * 3] = {
                -0.75f, -0.75f, 0.0f, // 0
                0.75f, -0.75f, 0.0f,  // 1
                0.75f, 0.75f, 0.0f,   // 2
                -0.75f, 0.75f, 0.0f,  // 3
            };
            std::shared_ptr<hazel::renderer::VertexBuffer> square_vertex_buffer;
            square_vertex_buffer.reset(hazel::renderer::VertexBuffer::create(vertices, sizeof(vertices)));
            square_vertex_buffer->set_layout({
                {hazel::renderer::ShaderDataType::FLOAT3, "a_Position"},
            });
            this->square_vertex_array->add_vertex_buffer(square_vertex_buffer);

            const unsigned int indices_len = 6;
            unsigned int indices[indices_len] = {0, 1, 2, 2, 3, 0};
            std::shared_ptr<hazel::renderer::IndexBuffer> square_index_buffer;
            square_index_buffer.reset(hazel::renderer::IndexBuffer::create(indices, indices_len));
            this->square_vertex_array->set_index_buffer(square_index_buffer);

            std::string vertex_source = R"(
                #version 450 core
                layout(location = 0) in vec3 a_Position;
                
                uniform mat4 u_ViewProjection;

                out vec3 v_Position;

                void main() {
                    v_Position = a_Position;
                    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
                }
            )";
            std::string fragment_source = R"(
                #version 450 core
                layout(location = 0) out vec4 color;

                in vec3 v_Position;

                void main() {
                    color = vec4(0.2, 0.3, 0.8, 1.0);
                }
            )";
            this->square_shader.reset(new hazel::renderer::Shader(vertex_source, fragment_source));
        }
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
            renderer::Command::set_clear_color({0.2, 0.2, 0.2, 1});
            renderer::Command::clear_color();

            this->camera->set_position({0.5f, 0.5f, 0.0f});
            this->camera->set_rotation(60.0f);

            renderer::Renderer::begin_scene(*this->camera);
            renderer::Renderer::submit(this->square_shader, this->square_vertex_array);
            renderer::Renderer::submit(this->shader, this->vertex_array);
            renderer::Renderer::end_scene();

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
