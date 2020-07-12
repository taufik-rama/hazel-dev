#include <hazel/renderer/command.hpp>

#include <hazel/platform/linux/opengl_library.hpp>

namespace hazel::renderer
{
    Library *Command::library = new hazel::platform::linux::Library();

    void Command::set_clear_color(const glm::vec4 &color)
    {
        library->set_clear_color(color);
    }

    void Command::clear_color()
    {
        library->clear_color();
    }

    void Command::draw_index(const std::shared_ptr<hazel::renderer::VertexArray> &va)
    {
        library->draw_index(va);
    }
} // namespace hazel::renderer