#pragma once

#include <hazel/renderer/vertex_array.hpp>

#include <glm/glm.hpp>

namespace hazel::renderer {
class Library {
public:
  enum class API {
    NONE = 0,
    OPENGL = 1,
  };

  static Library::API get_api_library() { return api_library; }

  virtual void init() = 0;

  virtual void set_clear_color(const glm::vec4 &color) = 0;

  virtual void set_viewport(unsigned int x, unsigned int y, unsigned int width,
                            unsigned int height) = 0;

  virtual void clear_color() = 0;

  virtual void draw_index(const hazel::core::Ref<VertexArray> &va) = 0;

private:
  static Library::API api_library;
};
} // namespace hazel::renderer