#include <hazel/renderer/command.hpp>

#include <hazel/platform/linux/opengl_library.hpp>

namespace hazel::renderer {
hazel::core::Ref<Library> Command::library =
    std::make_shared<hazel::platform::linux::Library>();

void Command::init() { library->init(); }

void Command::shutdown() {} /* noop */

void Command::set_clear_color(const glm::vec4 &color) {
  library->set_clear_color(color);
}

void Command::clear_color() { library->clear_color(); }

void Command::draw_index(
    const hazel::core::Ref<hazel::renderer::VertexArray> &va) {
  library->draw_index(va);
}

void Command::set_viewport(unsigned int x, unsigned int y, unsigned int width,
                           unsigned int height) {
  library->set_viewport(x, y, width, height);
}
} // namespace hazel::renderer