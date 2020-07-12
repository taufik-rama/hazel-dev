#include <hazel/renderer/renderer.hpp>

#include <hazel/renderer/command.hpp>

namespace hazel::renderer
{
    void Renderer::begin_scene()
    {
    }

    void Renderer::end_scene()
    {
    }

    void Renderer::submit(const std::shared_ptr<VertexArray> &va)
    {
        va->bind();
        va->get_index_buffer()->bind();
        Command::draw_index(va);
        va->unbind();
        va->get_index_buffer()->unbind();
    }
} // namespace hazel::renderer