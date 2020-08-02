#include <iostream>

#include <hazel-api.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

class ExampleLayer : public hazel::layer::Layer
{
public:
    ExampleLayer()
        : hazel::layer::Layer("Sandbox Example Layer"),
          camera_controller((float)16 / (float)9, true) // 16:9 aspect ratio
    {
        // Set the vsync to false so that we're able to see the actual framerate.
        // The app should feel/run normal whether this is on or off
        hazel::Application::get_application()->get_window().set_vsync(false);

        // Set camera translation & rotation behaviour.
        // For 2D/Orthogonal camera, pick which one is translated first,
        // the position or rotation (result will be different)
        this->camera_controller.get_camera().set_translation_rotation(hazel::camera::Orthographic::TRANSLATION_ROTATION::TRANSLATE_BEFORE_ROTATE);

        this->vertex_array = hazel::renderer::VertexArray::create();
        {
            float vertices[7 * 3] = {
                -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, // 0
                0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f,  // 1
                0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,   // 2
            };
            hazel::Ref<hazel::renderer::VertexBuffer> vertex_buffer;
            vertex_buffer = hazel::renderer::VertexBuffer::create(vertices, sizeof(vertices));
            {
                hazel::renderer::BufferLayout layout = {
                    {hazel::renderer::ShaderDataType::FLOAT3, "a_Position"},
                    {hazel::renderer::ShaderDataType::FLOAT4, "a_Color"},
                };
                vertex_buffer->set_layout(layout);
            }
            this->vertex_array->add_vertex_buffer(vertex_buffer);
        }
        {
            const unsigned int indices_len = 3;
            unsigned int indices[indices_len] = {0, 1, 2};
            hazel::Ref<hazel::renderer::IndexBuffer> index_buffer;
            index_buffer = hazel::renderer::IndexBuffer::create(indices, indices_len);
            this->vertex_array->set_index_buffer(index_buffer);
        }

        {
            std::string vertex_source = R"(
                #version 450 core
                layout(location = 0) in vec3 a_Position;
                layout(location = 1) in vec4 a_Color;

                uniform mat4 u_ViewProjection;
                uniform mat4 u_Transform;
                
                out vec3 v_Position;
                out vec4 v_Color;

                void main() {
                    v_Position = a_Position;
                    v_Color = a_Color;
                    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
                }
            )";
            std::string fragment_source = R"(
                #version 450 core
                layout(location = 0) out vec4 color;

                in vec3 v_Position;
                in vec4 v_Color;

                void main() {
                    color = v_Color;
                }
            )";
            this->shader_array.load("triangle_shader", vertex_source, fragment_source);
        }

        {
            this->square_vertex_array = hazel::renderer::VertexArray::create();

            float vertices[4 * 5] = {
                -0.5f, -0.5f, 0.0f, /* <- position, texture -> */ 0.0f, 0.0f, // 0
                0.5f, -0.5f, 0.0f, /* <- position, texture -> */ 1.0f, 0.0f,  // 1
                0.5f, 0.5f, 0.0f, /* <- position, texture -> */ 1.0f, 1.0f,   // 2
                -0.5f, 0.5f, 0.0f, /* <- position, texture -> */ 0.0f, 1.0f,  // 3
            };
            hazel::Ref<hazel::renderer::VertexBuffer> square_vertex_buffer;
            square_vertex_buffer = hazel::renderer::VertexBuffer::create(vertices, sizeof(vertices));
            square_vertex_buffer->set_layout({
                {hazel::renderer::ShaderDataType::FLOAT3, "a_Position"},
                {hazel::renderer::ShaderDataType::FLOAT2, "a_TexCoord"},
            });
            this->square_vertex_array->add_vertex_buffer(square_vertex_buffer);

            const unsigned int indices_len = 6;
            unsigned int indices[indices_len] = {0, 1, 2, 2, 3, 0};
            hazel::Ref<hazel::renderer::IndexBuffer> square_index_buffer;
            square_index_buffer = hazel::renderer::IndexBuffer::create(indices, indices_len);
            this->square_vertex_array->set_index_buffer(square_index_buffer);

            std::string vertex_source = R"(
                #version 450 core
                layout(location = 0) in vec3 a_Position;
                
                uniform mat4 u_ViewProjection;
                uniform mat4 u_Transform;

                void main() {
                    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
                }
            )";
            std::string fragment_source = R"(
                #version 450 core
                layout(location = 0) out vec4 color;

                uniform vec3 u_Color;

                void main() {
                    color = vec4(u_Color, 1.0f);
                }
            )";
            this->shader_array.load("square_shader", vertex_source, fragment_source);

            // Textures
            {
                this->checkerboard_texture = hazel::renderer::Texture2D::create("../assets/textures/Checkerboard.png");
                this->logo_texture = hazel::renderer::Texture2D::create("../assets/textures/ChernoLogo.png");

                auto texture_shader = hazel::renderer::Shader::create("../assets/shaders/texture.glsl");
                texture_shader->bind();
                std::static_pointer_cast<hazel::platform::linux::Shader>(texture_shader)->upload_uniform("u_Texture", 0 /* texture slot */);
                texture_shader->unbind();
                this->shader_array.add(texture_shader);
            }
        }
    }

    void on_update(hazel::core::Timestep ts) override
    {
        this->camera_controller.on_update(ts);

        hazel::renderer::Command::set_clear_color({0.2, 0.2, 0.2, 1});
        hazel::renderer::Command::clear_color();

        hazel::renderer::Renderer::begin_scene(this->camera_controller.get_camera());

        this->shader_array.get("square_shader")->bind();
        std::static_pointer_cast<hazel::platform::linux::Shader>(this->shader_array.get("square_shader"))->upload_uniform("u_Color", this->square_array);
        this->shader_array.get("square_shader")->unbind();

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
                hazel::renderer::Renderer::submit(
                    this->shader_array.get("square_shader"),
                    this->square_vertex_array,
                    glm::translate(glm::mat4(1.0f), pos) * scale);
            }
        }

        this->checkerboard_texture->bind(0);
        hazel::renderer::Renderer::submit(
            this->shader_array.get("texture"),
            this->square_vertex_array,
            glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        this->checkerboard_texture->unbind(0);

        this->logo_texture->bind(0);
        hazel::renderer::Renderer::submit(
            this->shader_array.get("texture"),
            this->square_vertex_array,
            glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        this->logo_texture->unbind(0);

        // Triangle
        hazel::renderer::Renderer::submit(this->shader_array.get("triangle_shader"), this->vertex_array);

        hazel::renderer::Renderer::end_scene();
    }

    virtual void on_imgui_render() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Array Color", glm::value_ptr(this->square_array));
        ImGui::End();
    }

    void on_event(hazel::event::Event &e) override
    {
        this->camera_controller.on_event(e);
    }

private:
    hazel::renderer::ShaderArray shader_array;

    hazel::Ref<hazel::renderer::VertexArray> vertex_array;
    hazel::Ref<hazel::renderer::VertexArray> square_vertex_array;
    hazel::Ref<hazel::renderer::Texture> checkerboard_texture, logo_texture;

    hazel::camera::OrthographicController camera_controller;

    glm::vec3 square_array = {0.2f, 0.3f, 0.8f};
};

class Sandbox : public hazel::Application
{
public:
    Sandbox()
    {
        this->add_layer(new ExampleLayer());
    }

    ~Sandbox() {}
};

hazel::Application *hazel::create_application()
{
    return new Sandbox();
}
