#pragma once

#include <hazel/renderer/graphics_context.hpp>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace hazel::renderer {
class OpenGLContext : public GraphicsContext {
public:
  OpenGLContext(GLFWwindow *window) : window(window) {}

  virtual void init() override;

  virtual void swap_buffers() override;

private:
  GLFWwindow *window;
};
} // namespace hazel::renderer