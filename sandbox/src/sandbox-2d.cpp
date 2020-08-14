#include <sandbox-2d.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

Sandbox2D::Sandbox2D()
    : hazel::layer::Layer("Sandbox2D Example Layer"),
      camera_controller((float)16 / (float)9, true) {} // 16:9 aspect ratio

Sandbox2D::~Sandbox2D() {}

void Sandbox2D::on_attach() {
  this->vertex_array = hazel::renderer::VertexArray::create();

  float vertices[4 * 5] = {
      -0.5f, -0.5f, 0.0f, // 0
      0.5f,  -0.5f, 0.0f, // 1
      0.5f,  0.5f,  0.0f, // 2
      -0.5f, 0.5f,  0.0f, // 3
  };
  hazel::core::Ref<hazel::renderer::VertexBuffer> square_vertex_buffer;
  square_vertex_buffer =
      hazel::renderer::VertexBuffer::create(vertices, sizeof(vertices));
  square_vertex_buffer->set_layout({
      {hazel::renderer::ShaderDataType::FLOAT3, "a_Position"},
  });
  this->vertex_array->add_vertex_buffer(square_vertex_buffer);

  const unsigned int indices_len = 6;
  unsigned int indices[indices_len] = {0, 1, 2, 2, 3, 0};
  hazel::core::Ref<hazel::renderer::IndexBuffer> square_index_buffer;
  square_index_buffer =
      hazel::renderer::IndexBuffer::create(indices, indices_len);
  this->vertex_array->set_index_buffer(square_index_buffer);

  this->shader =
      hazel::renderer::Shader::create("../assets/shaders/flat-color.glsl");
}

void Sandbox2D::on_detach() {}

void Sandbox2D::on_update(hazel::core::Timestep ts) {
  this->camera_controller.on_update(ts);

  hazel::renderer::Command::set_clear_color({0.2, 0.2, 0.2, 1});
  hazel::renderer::Command::clear_color();

  hazel::renderer::Renderer::begin_scene(this->camera_controller.get_camera());

  this->shader->bind();
  std::static_pointer_cast<hazel::platform::linux::Shader>(this->shader)
      ->upload_uniform("u_Color", this->square_array);
  this->shader->unbind();

  hazel::renderer::Renderer::submit(this->shader, this->vertex_array);

  hazel::renderer::Renderer::end_scene();
}

void Sandbox2D::on_event(hazel::event::Event &e) {
  this->camera_controller.on_event(e);
}

void Sandbox2D::on_imgui_render() {
  ImGui::Begin("Settings");
  ImGui::ColorEdit4("Square Array Color", glm::value_ptr(this->square_array));
  ImGui::End();
}
