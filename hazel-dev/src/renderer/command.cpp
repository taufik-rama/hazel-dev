#include <hazel/renderer/command.hpp>

#include <hazel/platform/linux/opengl_library.hpp>

namespace hazel::renderer
{
    Library *Command::api_library = new hazel::platform::linux::Library();

    void Command::set_clear_color(const glm::vec4 &color)
    {
        api_library->set_clear_color(color);
    }

    void Command::clear_color()
    {
        api_library->clear_color();
    }

    void Command::draw_index(const std::shared_ptr<hazel::renderer::VertexArray> &va)
    {
        api_library->draw_index(va);
    }
} // namespace hazel::renderer