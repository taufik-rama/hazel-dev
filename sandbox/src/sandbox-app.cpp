#include <iostream>

#include <hazel-api.hpp>

class ExampleLayer : public hazel::layer::Layer
{
public:
    ExampleLayer()
        : hazel::layer::Layer("Sandbox Example Layer"),
          camera_pos(0.0f),
          camera_pos_speed(0.05f),
          camera_rot(0.0f),
          camera_rot_speed(2.0f)
    {

        // 16:9 aspect ratio
        this->camera.reset(new hazel::camera::Orthographic(-1.6f, 1.6f, -0.9f, 0.9f));

        this->vertex_array.reset(hazel::renderer::VertexArray::create());
        {
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
        }
        {
            const unsigned int indices_len = 3;
            unsigned int indices[indices_len] = {0, 1, 2};
            std::shared_ptr<hazel::renderer::IndexBuffer> index_buffer;
            index_buffer.reset(hazel::renderer::IndexBuffer::create(indices, indices_len));
            this->vertex_array->set_index_buffer(index_buffer);
        }

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

    void on_update() override
    {
        if (hazel::input::Input::is_key_pressed(HAZEL_KEY_W))
        {
            this->camera_pos.y += this->camera_pos_speed;
        }
        else if (hazel::input::Input::is_key_pressed(HAZEL_KEY_S))
        {
            this->camera_pos.y -= this->camera_pos_speed;
        }

        if (hazel::input::Input::is_key_pressed(HAZEL_KEY_A))
        {
            this->camera_pos.x -= this->camera_pos_speed;
        }
        else if (hazel::input::Input::is_key_pressed(HAZEL_KEY_D))
        {
            this->camera_pos.x += this->camera_pos_speed;
        }

        if (hazel::input::Input::is_key_pressed(HAZEL_KEY_J))
        {
            this->camera_rot += this->camera_rot_speed;
        }
        else if (hazel::input::Input::is_key_pressed(HAZEL_KEY_L))
        {
            this->camera_rot -= this->camera_rot_speed;
        }

        hazel::renderer::Command::set_clear_color({0.2, 0.2, 0.2, 1});
        hazel::renderer::Command::clear_color();

        this->camera->set_position(this->camera_pos);
        this->camera->set_rotation(this->camera_rot);

        hazel::renderer::Renderer::begin_scene(*this->camera);
        hazel::renderer::Renderer::submit(this->square_shader, this->square_vertex_array);
        hazel::renderer::Renderer::submit(this->shader, this->vertex_array);
        hazel::renderer::Renderer::end_scene();
    }

    void on_event(hazel::event::Event &) override
    {
    }

private:
    std::shared_ptr<hazel::renderer::Shader> shader;
    std::shared_ptr<hazel::renderer::VertexArray> vertex_array;
    std::shared_ptr<hazel::renderer::Shader> square_shader;
    std::shared_ptr<hazel::renderer::VertexArray> square_vertex_array;
    std::shared_ptr<hazel::camera::Orthographic> camera;

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
