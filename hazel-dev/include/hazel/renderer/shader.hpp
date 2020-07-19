#pragma once

#include <hazel/core.hpp>

namespace hazel::renderer
{
    enum class ShaderDataType : unsigned int
    {
        NONE = 0,
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
    };

    inline static unsigned int shader_data_type_size(ShaderDataType t)
    {
        switch (t)
        {
        case ShaderDataType::NONE:
            return 0;
        case ShaderDataType::FLOAT:
            return 4;
        case ShaderDataType::FLOAT2:
            return 4 * 2;
        case ShaderDataType::FLOAT3:
            return 4 * 3;
        case ShaderDataType::FLOAT4:
            return 4 * 4;
        }
        assert(false);
    }

    class Shader
    {
    public:
        virtual void bind() const = 0;

        virtual void unbind() const = 0;

        static hazel::Ref<Shader> create(std::string &vertex_source, std::string &fragment_source);

        virtual std::string to_string() const = 0;
        FORMAT_TO_STRING(Shader)
    };
} // namespace hazel::renderer
