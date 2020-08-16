#include <hazel/renderer/renderer2D.hpp>

#include <hazel/renderer/command.hpp>
#include <hazel/renderer/vertex_array.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace hazel::renderer {
struct Renderer2DStorage {
  hazel::core::Ref<hazel::renderer::VertexArray> vertex_array;
  hazel::core::Ref<hazel::renderer::Shader> shader;
  hazel::core::Ref<hazel::renderer::Texture2D> white_texture;
};

static Renderer2DStorage *storage;

void Renderer2D::init() {
  Command::init();

  storage = new Renderer2DStorage();

  storage->vertex_array = hazel::renderer::VertexArray::create();
  {
    float vertices[4 * 5] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 0
        1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // 1
        1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // 2
        -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // 3
    };
    hazel::core::Ref<hazel::renderer::VertexBuffer> square_vertex_buffer;
    square_vertex_buffer =
        hazel::renderer::VertexBuffer::create(vertices, sizeof(vertices));
    square_vertex_buffer->set_layout({
        {hazel::renderer::ShaderDataType::FLOAT3, "a_Position"},
        {hazel::renderer::ShaderDataType::FLOAT2, "a_TexCoord"},
    });
    storage->vertex_array->add_vertex_buffer(square_vertex_buffer);
  }
  {
    const unsigned int indices_len = 6;
    unsigned int indices[indices_len] = {0, 1, 2, 2, 3, 0};
    hazel::core::Ref<hazel::renderer::IndexBuffer> square_index_buffer;
    square_index_buffer =
        hazel::renderer::IndexBuffer::create(indices, indices_len);
    storage->vertex_array->set_index_buffer(square_index_buffer);
  }

  unsigned int data = 0xFFFFFFFF; // 1 pixel full-white RGBA color
  storage->white_texture = hazel::renderer::Texture2D::create(&data, 1, 1);

  storage->shader =
      hazel::renderer::Shader::create("../assets/shaders/texture-color.glsl");
  storage->shader->bind();
  storage->shader->set_uniform("u_Texture", 0);
  storage->shader->unbind();
}

void Renderer2D::shutdown() {
  Command::shutdown();
  delete storage;
}

void Renderer2D::begin_scene(const hazel::camera::Orthographic &camera) {
  storage->shader->bind();
  storage->shader->set_uniform("u_ViewProjection",
                               camera.get_projection_view_matrix());
  storage->shader->unbind();
}

void Renderer2D::end_scene() {}

void Renderer2D::draw_quad(const glm::vec2 &position, const float &rotation,
                           const glm::vec2 &size, const glm::vec4 &color) {
  Renderer2D::draw_quad({position.x, position.y, 0}, rotation, size, color);
}

void Renderer2D::draw_quad(const glm::vec3 &position, const float &rotation,
                           const glm::vec2 &size, const glm::vec4 &color) {
  storage->white_texture->bind(0);
  storage->shader->bind();
  storage->shader->set_uniform("u_Color", color);

  {
    auto t = glm::translate(glm::mat4(1.0f), position);
    auto r = glm::rotate(glm::mat4(1.0f), glm::radians(rotation),
                         glm::vec3(0, 0, 1));
    auto s = glm::scale(glm::mat4(1.0f), {size.x, size.y, 0.0f});
    storage->shader->set_uniform("u_Transform", t * r * s);
  }

  storage->vertex_array->bind();
  storage->vertex_array->get_index_buffer()->bind();
  Command::draw_index(storage->vertex_array);
  storage->vertex_array->get_index_buffer()->unbind();
  storage->vertex_array->unbind();

  storage->shader->unbind();
  storage->white_texture->unbind(0);
}

void Renderer2D::draw_quad(const glm::vec2 &position, const float &rotation,
                           const glm::vec2 &size,
                           const hazel::core::Ref<Texture2D> &texture,
                           const glm::vec4 &color) {
  Renderer2D::draw_quad({position.x, position.y, 0}, rotation, size, texture,
                        color);
}

void Renderer2D::draw_quad(const glm::vec3 &position, const float &rotation,
                           const glm::vec2 &size,
                           const hazel::core::Ref<Texture2D> &texture,
                           const glm::vec4 &color) {
  texture->bind(0);
  storage->shader->bind();
  storage->shader->set_uniform("u_Color", color);

  {
    auto t = glm::translate(glm::mat4(1.0f), position);
    auto r = glm::rotate(glm::mat4(1.0f), glm::radians(rotation),
                         glm::vec3(0, 0, 1));
    auto s = glm::scale(glm::mat4(1.0f), {size.x, size.y, 0.0f});
    storage->shader->set_uniform("u_Transform", t * r * s);
  }

  storage->vertex_array->bind();
  storage->vertex_array->get_index_buffer()->bind();
  Command::draw_index(storage->vertex_array);
  storage->vertex_array->get_index_buffer()->unbind();
  storage->vertex_array->unbind();

  storage->shader->unbind();
  texture->unbind(0);
}

} // namespace hazel::renderer