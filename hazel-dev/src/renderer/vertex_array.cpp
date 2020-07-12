#include <hazel/renderer/vertex_array.hpp>

#include <hazel/platform/linux/opengl_vertex_array.hpp>
#include <hazel/renderer/library.hpp>

namespace hazel::renderer
{
    VertexArray *VertexArray::create()
    {
        switch (Library::get_api_library())
        {
        case Library::API::NONE:
            break;

        case Library::API::OPENGL:
            return new hazel::platform::linux::VertexArray();
        }
        assert(false);
    }
} // namespace hazel::renderer
