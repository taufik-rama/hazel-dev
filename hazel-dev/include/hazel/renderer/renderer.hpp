#pragma once

#include <hazel/camera/_fwd.hpp>
#include <hazel/renderer/command.hpp>
#include <hazel/renderer/shader.hpp>
#include <hazel/renderer/vertex_array.hpp>

#include <glm/glm.hpp>

namespace hazel::renderer {
class Renderer {
public:
  static Library::API get_api_library() { return Library::get_api_library(); }

  static void init();

  static void shutdown();

  static void begin_scene(hazel::camera::Orthographic &);

  static void end_scene();

  static void resize_window(unsigned int width, unsigned int height);

  static void submit(const hazel::core::Ref<Shader> &,
                     const hazel::core::Ref<VertexArray> &,
                     const glm::mat4 & = glm::mat4(1.0f));

private:
  static struct SceneData { glm::mat4 projection_view_matrix; } scene_data;
};
} // namespace hazel::renderer