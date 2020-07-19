#pragma once

#include <hazel/renderer/shader.hpp>

#include <glm/glm.hpp>

namespace hazel::platform::linux
{
    class Shader : public hazel::renderer::Shader
    {
    public:
        Shader(std::string &vertex_source, std::string &fragment_source);

        virtual ~Shader();

        void bind() const override;

        void unbind() const override;

        void upload_uniform(const std::string &, const int &);

        void upload_uniform(const std::string &, const glm::vec3 &);

        void upload_uniform(const std::string &, const glm::vec4 &);

        void upload_uniform(const std::string &, const glm::mat4 &);

        virtual std::string to_string() const override;

    private:
        unsigned int renderer_id;
    };
} // namespace hazel::platform::linux
