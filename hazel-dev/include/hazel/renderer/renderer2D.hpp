#pragma once

#include <hazel/camera/orthographic.hpp>

namespace hazel::renderer {
class Renderer2D {
public:
  static void init();

  static void shutdown();

  static void begin_scene(const hazel::camera::Orthographic &);

  static void end_scene();

  static void draw_quad(const glm::vec2 &position, const glm::vec2 &size,
                        const glm::vec4 &color);

  static void draw_quad(const glm::vec3 &position, const glm::vec2 &size,
                        const glm::vec4 &color);
};
} // namespace hazel::renderer