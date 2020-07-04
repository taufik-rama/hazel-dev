#pragma once

#include <hazel/core.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace hazel::renderer
{
    class Shader
    {
    public:
        Shader(std::string &vertex_source, std::string &fragment_source);

        ~Shader();

        void bind() const;

        void unbind() const;

    private:
        unsigned int renderer_id;
    };
} // namespace hazel::renderer
