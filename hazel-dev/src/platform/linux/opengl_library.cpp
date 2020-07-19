#include <hazel/platform/linux/opengl_library.hpp>

#include <hazel/platform/linux/opengl_log.hpp>

#include <glad/glad.h>

namespace hazel::platform::linux
{
    void Library::set_clear_color(const glm::vec4 &color)
    {
        gl_call(glClearColor(color.r, color.g, color.b, color.a));
    }

    void Library::clear_color()
    {
        gl_call(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void Library::draw_index(const hazel::Ref<hazel::renderer::VertexArray> &va)
    {
        gl_call(glDrawElements(GL_TRIANGLES, va->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr));
    }
} // namespace hazel::platform::linux