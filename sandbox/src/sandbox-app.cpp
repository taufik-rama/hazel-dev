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
          camera_pos(0.0f),
          camera_pos_speed(5.0f),
          camera_rot(0.0f),
          camera_rot_speed(180.0f)
    {
        // Set the vsync to false so that we're able to see the actual framerate.
        // The app should feel/run normal whether this is on or off
        hazel::Application::get_application()->get_window().set_vsync(false);

        // 16:9 aspect ratio
        this->camera.reset(new hazel::camera::Orthographic(-1.6f, 1.6f, -0.9f, 0.9f));
        this->camera->set_translation_rotation(hazel::camera::Orthographic::TRANSLATION_ROTATION::TRANSLATE_BEFORE_ROTATE);

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
            this->shader = hazel::renderer::Shader::create(vertex_source, fragment_source);
        }

        {
            this->square_vertex_array = hazel::renderer::VertexArray::create();

            float vertices[4 * 5] = {
                -0.5f, -0.5f, 0.0f, /* texture */ 0.0f, 0.0f, // 0
                0.5f, -0.5f, 0.0f, /* texture */ 1.0f, 0.0f,  // 1
                0.5f, 0.5f, 0.0f, /* texture */ 1.0f, 1.0f,   // 2
                -0.5f, 0.5f, 0.0f, /* texture */ 0.0f, 1.0f,  // 3
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
            this->square_shader = hazel::renderer::Shader::create(vertex_source, fragment_source);

            // Texture
            {
                this->texture_shader = hazel::renderer::Shader::create("../assets/shaders/texture.glsl");

                this->texture = hazel::renderer::Texture2D::create("../assets/textures/Checkerboard.png");
                this->logo_texture = hazel::renderer::Texture2D::create("../assets/textures/ChernoLogo.png");

                this->texture_shader->bind();
                std::static_pointer_cast<hazel::platform::linux::Shader>(this->texture_shader)->upload_uniform("u_Texture", 0 /* texture slot */);
            }
        }
    }

    void on_update(hazel::core::Timestep ts) override
    {
        if (hazel::input::Input::is_key_pressed(HAZEL_KEY_W))
        {
            this->camera_pos.y += this->camera_pos_speed * ts;
        }
        else if (hazel::input::Input::is_key_pressed(HAZEL_KEY_S))
        {
            this->camera_pos.y -= this->camera_pos_speed * ts;
        }

        if (hazel::input::Input::is_key_pressed(HAZEL_KEY_A))
        {
            this->camera_pos.x -= this->camera_pos_speed * ts;
        }
        else if (hazel::input::Input::is_key_pressed(HAZEL_KEY_D))
        {
            this->camera_pos.x += this->camera_pos_speed * ts;
        }

        if (hazel::input::Input::is_key_pressed(HAZEL_KEY_O))
        {
            this->camera_rot += this->camera_rot_speed * ts;
        }
        else if (hazel::input::Input::is_key_pressed(HAZEL_KEY_P))
        {
            this->camera_rot -= this->camera_rot_speed * ts;
        }

        hazel::renderer::Command::set_clear_color({0.2, 0.2, 0.2, 1});
        hazel::renderer::Command::clear_color();

        this->camera->set_position(this->camera_pos);
        this->camera->set_rotation(this->camera_rot);

        hazel::renderer::Renderer::begin_scene(*this->camera);

        this->square_shader->bind();
        std::static_pointer_cast<hazel::platform::linux::Shader>(this->square_shader)->upload_uniform("u_Color", this->square_color);
        this->square_shader->unbind();

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
                hazel::renderer::Renderer::submit(
                    this->square_shader,
                    this->square_vertex_array,
                    glm::translate(glm::mat4(1.0f), pos) * scale);
            }
        }

        this->texture->bind(0);
        hazel::renderer::Renderer::submit(
            this->texture_shader,
            this->square_vertex_array,
            glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        this->texture->unbind(0);

        this->logo_texture->bind(0);
        hazel::renderer::Renderer::submit(
            this->texture_shader,
            this->square_vertex_array,
            glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        this->logo_texture->unbind(0);

        // Triangle
        // hazel::renderer::Renderer::submit(this->shader, this->vertex_array);

        hazel::renderer::Renderer::end_scene();
    }

    virtual void on_imgui_render() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(this->square_color));
        ImGui::End();
    }

    void on_event(hazel::event::Event &) override
    {
    }

private:
    hazel::Ref<hazel::renderer::Shader> shader;
    hazel::Ref<hazel::renderer::VertexArray> vertex_array;

    hazel::Ref<hazel::renderer::Shader> square_shader, texture_shader;
    hazel::Ref<hazel::renderer::VertexArray> square_vertex_array;
    glm::vec3 square_color = {0.2f, 0.3f, 0.8f};

    hazel::Ref<hazel::renderer::Texture> texture, logo_texture;

    hazel::Ref<hazel::camera::Orthographic> camera;
    glm::vec3 camera_pos;
    float camera_pos_speed;
    float camera_rot;
    float camera_rot_speed;
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
