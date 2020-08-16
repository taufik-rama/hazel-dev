#include <hazel/renderer/renderer.hpp>

#include <hazel/camera/orthographic.hpp>
#include <hazel/renderer/command.hpp>

namespace hazel::renderer {
Renderer::SceneData Renderer::scene_data;

void Renderer::init() { Command::init(); }

void Renderer::shutdown() { Command::shutdown(); }

void Renderer::begin_scene(hazel::camera::Orthographic &camera) {
  scene_data.projection_view_matrix = camera.get_projection_view_matrix();
}

void Renderer::end_scene() {}

void Renderer::submit(const hazel::core::Ref<Shader> &shader,
                      const hazel::core::Ref<VertexArray> &va,
                      const glm::mat4 &transform) {
  shader->bind();
  shader->set_uniform("u_ViewProjection", scene_data.projection_view_matrix);
  shader->set_uniform("u_Transform", transform);

  va->bind();
  va->get_index_buffer()->bind();
  Command::draw_index(va);
  va->unbind();
  va->get_index_buffer()->unbind();

  shader->unbind();
}

void Renderer::resize_window(unsigned int width, unsigned int height) {
  Command::set_viewport(0, 0, width, height);
}
} // namespace hazel::renderer