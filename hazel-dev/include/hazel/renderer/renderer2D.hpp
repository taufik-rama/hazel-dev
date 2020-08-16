#pragma once

#include <hazel/camera/orthographic.hpp>
#include <hazel/renderer/texture.hpp>

namespace hazel::renderer {
class Renderer2D {
public:
  static void init();

  static void shutdown();

  static void begin_scene(const hazel::camera::Orthographic &);

  static void end_scene();

  static void draw_quad(const glm::vec2 &position, const float &rotation,
                        const glm::vec2 &size, const glm::vec4 &color);

  static void draw_quad(const glm::vec3 &position, const float &rotation,
                        const glm::vec2 &size, const glm::vec4 &color);

  static void draw_quad(const glm::vec2 &position, const float &rotation,
                        const glm::vec2 &size,
                        const hazel::core::Ref<Texture2D> &texture,
                        const glm::vec4 &color = glm::vec4(1.0f));

  static void draw_quad(const glm::vec3 &position, const float &rotation,
                        const glm::vec2 &size,
                        const hazel::core::Ref<Texture2D> &texture,
                        const glm::vec4 &color = glm::vec4(1.0f));
};
} // namespace hazel::renderer