#include <hazel/application.hpp>

#include <hazel/event/window.hpp>
#include <hazel/layer/collection.hpp>
#include <hazel/layer/imgui.hpp>
#include <hazel/renderer/buffer.hpp>
#include <hazel/renderer/shader.hpp>

// Temporary, GL stuff
#include <glad/glad.h>
#include <hazel/platform/linux/opengl_log.hpp>

namespace hazel
{
    Application *Application::instance = nullptr;

    static GLenum shader_data_type_to_opengl_type(hazel::renderer::ShaderDataType t)
    {
        switch (t)
        {
        case hazel::renderer::ShaderDataType::FLOAT:
            return GL_FLOAT;
        case hazel::renderer::ShaderDataType::FLOAT2:
            return GL_FLOAT;
        case hazel::renderer::ShaderDataType::FLOAT3:
            return GL_FLOAT;
        case hazel::renderer::ShaderDataType::FLOAT4:
            return GL_FLOAT;
        default:
            break;
        }
        assert(false);
    }

    Application::Application()
    {
        assert(!Application::instance);
        Application::instance = this;

        this->layers = new hazel::layer::Collection();

        this->window = std::unique_ptr<Window>(Window::create());
        this->window->set_event_callback(std::bind(&Application::event_callback, this, std::placeholders::_1));

        this->imgui = new hazel::layer::ImGui();
        this->add_layer_overlay(this->imgui);

        gl_call(glCreateVertexArrays(1, &this->vertex_array));
        gl_call(glBindVertexArray(this->vertex_array));

        float vertices[7 * 3] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, // 0
            0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f,  // 1
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,   // 2
        };

        this->vertex_buffer.reset(hazel::renderer::VertexBuffer::create(vertices, sizeof(vertices)));

        {
            hazel::renderer::BufferLayout layout = {
                {hazel::renderer::ShaderDataType::FLOAT3, "a_Position"},
                {hazel::renderer::ShaderDataType::FLOAT4, "a_Color"},
            };
            this->vertex_buffer->set_layout(layout);
        }

        int index = 0;
        const auto &layout = this->vertex_buffer->get_layout();
        for (const auto &e : layout)
        {
            gl_call(glEnableVertexAttribArray(index));
            gl_call(glVertexAttribPointer(
                index,
                e.get_component_count(),
                shader_data_type_to_opengl_type(e.type),
                e.normalized ? GL_TRUE : GL_FALSE,
                layout.get_stride(),
                (const void *)e.offset));
            index++;
        }

        const unsigned int indices_len = 3;
        unsigned int indices[indices_len] = {0, 1, 2};
        this->index_buffer.reset(hazel::renderer::IndexBuffer::create(indices, indices_len));

        std::string vertex_source = R"(
            #version 450 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;
            
            out vec3 v_Position;
            out vec4 v_Color;

            void main() {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
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
            gl_call(glClearColor(0.2, 0.2, 0.2, 1));
            gl_call(glClear(GL_COLOR_BUFFER_BIT));

            this->shader->bind();

            gl_call(glBindVertexArray(this->vertex_array));
            gl_call(glDrawElements(GL_TRIANGLES, this->index_buffer->get_count(), GL_UNSIGNED_INT, nullptr));

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
