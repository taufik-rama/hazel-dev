#include <hazel/renderer/vertex_array.hpp>

#include <hazel/platform/linux/opengl_vertex_array.hpp>
#include <hazel/renderer/renderer.hpp>

namespace hazel::renderer
{
    VertexArray* VertexArray::create()
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
} // namespace hazel::renderer
