#include <sandbox-2d.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

class ExampleLayer : public hazel::layer::Layer {
public:
  ExampleLayer()
      : hazel::layer::Layer("Sandbox Example Layer"),
        camera_controller((float)16 / (float)9, true) // 16:9 aspect ratio
  {
    TIMER_SCOPE();
    // Set the vsync to false so that we're able to see the actual framerate.
    // The app should feel/run normal whether this is on or off
    hazel::core::Application::get_application()->get_window().set_vsync(false);

    // Set camera translation & rotation behaviour.
    // For 2D/Orthogonal camera, pick which one is translated first,
    // the position or rotation (result will be different)
    this->camera_controller.get_camera().set_translation_rotation(
        hazel::camera::Orthographic::TRANSLATION_ROTATION::
            TRANSLATE_BEFORE_ROTATE);

    this->vertex_array = hazel::renderer::VertexArray::create();
    {
      float vertices[7 * 3] = {
          -0.5f, -0.5f, 0.1f, 0.8f, 0.2f, 0.8f, 1.0f, // 0
          0.5f,  -0.5f, 0.1f, 0.2f, 0.2f, 0.8f, 1.0f, // 1
          0.0f,  0.5f,  0.1f, 0.8f, 0.8f, 0.2f, 1.0f, // 2
      };
      hazel::core::Ref<hazel::renderer::VertexBuffer> vertex_buffer;
      vertex_buffer =
          hazel::renderer::VertexBuffer::create(vertices, sizeof(vertices));
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
      hazel::core::Ref<hazel::renderer::IndexBuffer> index_buffer;
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
      this->shader_array.load("triangle_shader", vertex_source,
                              fragment_source);
    }

    {
      this->square_vertex_array = hazel::renderer::VertexArray::create();
      {
        float vertices[4 * 5] = {
            -0.5f, -0.5f, 0.0f, /* <- position, texture -> */ 0.0f, 0.0f, // 0
            0.5f,  -0.5f, 0.0f, /* <- position, texture -> */ 1.0f, 0.0f, // 1
            0.5f,  0.5f,  0.0f, /* <- position, texture -> */ 1.0f, 1.0f, // 2
            -0.5f, 0.5f,  0.0f, /* <- position, texture -> */ 0.0f, 1.0f, // 3
        };
        hazel::core::Ref<hazel::renderer::VertexBuffer> square_vertex_buffer;
        square_vertex_buffer =
            hazel::renderer::VertexBuffer::create(vertices, sizeof(vertices));
        square_vertex_buffer->set_layout({
            {hazel::renderer::ShaderDataType::FLOAT3, "a_Position"},
            {hazel::renderer::ShaderDataType::FLOAT2, "a_TexCoord"},
        });
        this->square_vertex_array->add_vertex_buffer(square_vertex_buffer);

        const unsigned int indices_len = 6;
        unsigned int indices[indices_len] = {0, 1, 2, 2, 3, 0};
        hazel::core::Ref<hazel::renderer::IndexBuffer> square_index_buffer;
        square_index_buffer =
            hazel::renderer::IndexBuffer::create(indices, indices_len);
        this->square_vertex_array->set_index_buffer(square_index_buffer);
      }

      this->shader_array.load("square_shader",
                              "../assets/shaders/texture-color.glsl");

      // Textures
      {
        this->checkerboard_texture = hazel::renderer::Texture2D::create(
            "../assets/textures/Checkerboard.png");
        this->logo_texture = hazel::renderer::Texture2D::create(
            "../assets/textures/ChernoLogo.png");
      }
    }
  }

  void on_update(hazel::core::Timestep ts) override {
    TIMER_SCOPE();
    this->camera_controller.on_update(ts);

    hazel::renderer::Command::set_clear_color({0.2, 0.2, 0.2, 1});
    hazel::renderer::Command::clear_color();

    hazel::renderer::Renderer::begin_scene(
        this->camera_controller.get_camera());

    {
      TIMER_SCOPE_NAME("on_update(hazel::core::Timestep ts) - 1");
      this->shader_array.get("square_shader")->bind();
      this->shader_array.get("square_shader")
          ->set_uniform("u_Color", this->square_array);
      this->shader_array.get("square_shader")->set_uniform("u_Texture", 0);
      this->shader_array.get("square_shader")->unbind();
    }

    {
      TIMER_SCOPE_NAME("on_update(hazel::core::Timestep ts) - 2");
      this->checkerboard_texture->bind(0);
      glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
      for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
          glm::vec3 pos(i * 0.11f, j * 0.11f, -0.1f);
          hazel::renderer::Renderer::submit(
              this->shader_array.get("square_shader"),
              this->square_vertex_array,
              glm::translate(glm::mat4(1.0f), pos) * scale);
        }
      }
      this->checkerboard_texture->unbind(0);
    }

    {
      TIMER_SCOPE_NAME("on_update(hazel::core::Timestep ts) - 3");
      this->checkerboard_texture->bind(0);
      hazel::renderer::Renderer::submit(
          this->shader_array.get("square_shader"), this->square_vertex_array,
          glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
      this->checkerboard_texture->unbind(0);
    }

    {
      TIMER_SCOPE_NAME("on_update(hazel::core::Timestep ts) - 4");
      this->logo_texture->bind(0);
      hazel::renderer::Renderer::submit(
          this->shader_array.get("square_shader"), this->square_vertex_array,
          glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.05f}) *
              glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
      this->logo_texture->unbind(0);
    }

    // Triangle
    hazel::renderer::Renderer::submit(this->shader_array.get("triangle_shader"),
                                      this->vertex_array);

    hazel::renderer::Renderer::end_scene();
  }

  virtual void on_imgui_render() override {
    TIMER_SCOPE();
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square Array Color", glm::value_ptr(this->square_array));
    ImGui::End();
  }

  void on_event(hazel::event::Event &e) override {
    this->camera_controller.on_event(e);
  }

private:
  hazel::renderer::ShaderArray shader_array;

  hazel::core::Ref<hazel::renderer::VertexArray> vertex_array,
      square_vertex_array;
  hazel::core::Ref<hazel::renderer::Texture> checkerboard_texture, logo_texture;

  hazel::camera::OrthographicController camera_controller;

  glm::vec4 square_array = glm::vec4(1.0f);
};

class Sandbox : public hazel::core::Application {
public:
  Sandbox() {
    // this->add_layer(new ExampleLayer());
    this->sandbox2D = hazel::core::create_ref<Sandbox2D>();
    this->add_layer(this->sandbox2D);
  }

  ~Sandbox() {}

private:
  hazel::core::Ref<Sandbox2D> sandbox2D;
};

hazel::core::Scope<hazel::core::Application> hazel::core::create_application() {
  return hazel::core::create_scope<Sandbox>();
}
