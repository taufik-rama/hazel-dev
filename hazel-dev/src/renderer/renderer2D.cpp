#include <hazel/renderer/renderer2D.hpp>

#include <hazel/platform/linux/opengl_shader.hpp>
#include <hazel/renderer/command.hpp>
#include <hazel/renderer/vertex_array.hpp>

namespace hazel::renderer {
struct Renderer2DStorage {
  hazel::core::Ref<hazel::renderer::VertexArray> vertex_array;
  hazel::core::Ref<hazel::renderer::Shader> shader;
};

static Renderer2DStorage *storage;

void Renderer2D::init() {
  Command::init();

  storage = new Renderer2DStorage();

  storage->vertex_array = hazel::renderer::VertexArray::create();

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
  storage->vertex_array->add_vertex_buffer(square_vertex_buffer);

  const unsigned int indices_len = 6;
  unsigned int indices[indices_len] = {0, 1, 2, 2, 3, 0};
  hazel::core::Ref<hazel::renderer::IndexBuffer> square_index_buffer;
  square_index_buffer =
      hazel::renderer::IndexBuffer::create(indices, indices_len);
  storage->vertex_array->set_index_buffer(square_index_buffer);

  storage->shader =
      hazel::renderer::Shader::create("../assets/shaders/flat-color.glsl");
}

void Renderer2D::shutdown() {
  Command::shutdown();
  delete storage;
}

void Renderer2D::begin_scene(const hazel::camera::Orthographic &camera) {
  storage->shader->bind();
  std::static_pointer_cast<hazel::platform::linux::Shader>(storage->shader)
      ->upload_uniform("u_ViewProjection", camera.get_projection_view_matrix());
  std::static_pointer_cast<hazel::platform::linux::Shader>(storage->shader)
      ->upload_uniform("u_Transform", glm::mat4(1.0f));
  storage->shader->unbind();
}

void Renderer2D::end_scene() {}

void Renderer2D::draw_quad(const glm::vec2 &position, const glm::vec2 &size,
                           const glm::vec4 &color) {
  Renderer2D::draw_quad({position.x, position.y, 0}, size, color);
}

void Renderer2D::draw_quad(const glm::vec3 &, const glm::vec2 &,
                           const glm::vec4 &color) {
  storage->shader->bind();
  std::static_pointer_cast<hazel::platform::linux::Shader>(storage->shader)
      ->upload_uniform("u_Color", color);

  storage->vertex_array->bind();
  storage->vertex_array->get_index_buffer()->bind();
  Command::draw_index(storage->vertex_array);
  storage->vertex_array->get_index_buffer()->unbind();
  storage->vertex_array->unbind();

  storage->shader->unbind();
}

} // namespace hazel::renderer