#include <hazel/renderer/shader.hpp>

#include <hazel/platform/linux/opengl_shader.hpp>
#include <hazel/renderer/library.hpp>

namespace hazel::renderer
{
    hazel::Ref<Shader> Shader::create(const std::string &filepath)
    {
        switch (Library::get_api_library())
        {
        case Library::API::NONE:
            break;

        case Library::API::OPENGL:
            return std::make_shared<hazel::platform::linux::Shader>(filepath);
        }
        assert(false);
    }

    hazel::Ref<Shader> Shader::create(const std::string &vertex_source, const std::string &fragment_source)
    {
        switch (Library::get_api_library())
        {
        case Library::API::NONE:
            break;

        case Library::API::OPENGL:
            return std::make_shared<hazel::platform::linux::Shader>(vertex_source, fragment_source);
        }
        assert(false);
    }
} // namespace hazel::renderer
