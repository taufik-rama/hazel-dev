#include <hazel/renderer/texture.hpp>

#include <hazel/platform/linux/opengl_texture.hpp>
#include <hazel/renderer/library.hpp>

namespace hazel::renderer
{
    hazel::Ref<Texture2D> Texture2D::create(const std::string &path)
    {
        switch (Library::get_api_library())
        {
        case Library::API::NONE:
            break;

        case Library::API::OPENGL:
            return std::make_shared<hazel::platform::linux::Texture2D>(path);
        }
        assert(false);
    }
} // namespace hazel::renderer
