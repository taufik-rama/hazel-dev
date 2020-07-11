#include <hazel/renderer/vertex_array.hpp>

#include <hazel/platform/linux/opengl_vertex_array.hpp>
#include <hazel/renderer/renderer.hpp>

namespace hazel::renderer
{
    VertexArray::VertexArray *create()
    {
        switch (Renderer::get_api())
        {
        case RendererAPI::NONE:
            break;

        case RendererAPI::OPENGL:
            return new hazel::platform::linux::VertexArray();
        }
        assert(false);
    }

    void VertexArray::bind() const {}

    void VertexArray::unbind() const {}

    void VertexArray::add_vertex_buffer(const std::shared_ptr<VertexBuffer> &buffer) {}

    void VertexArray::add_index_buffer(const std::shared_ptr<IndexBuffer> &buffer) {}
} // namespace hazel::renderer
