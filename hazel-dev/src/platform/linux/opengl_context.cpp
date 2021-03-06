#include <hazel/platform/linux/opengl_context.hpp>

namespace hazel::renderer {
void OpenGLContext::init() {
  TIMER_SCOPE();
  glfwMakeContextCurrent(this->window);
  assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
  HAZEL_DEV_LOG_INFO(
      "\nOpenGL specs:\n  Version: {}\n  Renderer: {}\n  Vendor: {}",
      glGetString(GL_VERSION), glGetString(GL_RENDERER),
      glGetString(GL_VENDOR));
}

void OpenGLContext::swap_buffers() {
  TIMER_SCOPE();
  glfwSwapBuffers(this->window);
}
} // namespace hazel::renderer
