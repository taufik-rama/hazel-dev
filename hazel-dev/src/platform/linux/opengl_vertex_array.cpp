#include <hazel/platform/linux/opengl_vertex_array.hpp>

#include <hazel/platform/linux/opengl_log.hpp>

#include <glad/glad.h>

namespace hazel::platform::linux
{
    static GLenum shader_data_type_to_opengl_type(hazel::renderer::ShaderDataType t)
    {
        switch (t)
        {
        case hazel::renderer::ShaderDataType::FLOAT:
            return GL_FLOAT;
        case hazel::renderer::ShaderDataType::FLOAT2:
            return GL_FLOAT;
        case hazel::renderer::ShaderDataType::FLOAT3:
            return GL_FLOAT;
        case hazel::renderer::ShaderDataType::FLOAT4:
            return GL_FLOAT;
        default:
            break;
        }
        assert(false);
    }

    VertexArray::VertexArray()
    {
        gl_call(glCreateVertexArrays(1, &this->rendered_id));
        gl_call(glBindVertexArray(this->rendered_id));
    }

    void VertexArray::bind() const
    {
        gl_call(glBindVertexArray(this->rendered_id));
    }

    void VertexArray::unbind() const
    {
        gl_call(glBindVertexArray(0));
    }

    void VertexArray::add_vertex_buffer(const std::shared_ptr<hazel::renderer::VertexBuffer> &buffer)
    {
        gl_call(glBindVertexArray(this->rendered_id));
        buffer->bind();

        int index = 0;
        const auto &layout = buffer->get_layout();
        for (const auto &e : layout)
        {
            gl_call(glEnableVertexAttribArray(index));
            gl_call(glVertexAttribPointer(
                index,
                e.get_component_count(),
                shader_data_type_to_opengl_type(e.type),
                e.normalized ? GL_TRUE : GL_FALSE,
                layout.get_stride(),
                (const void *)e.offset));
            index++;
        }

        this->vertex_buffers.push_back(buffer);
    }

    void VertexArray::add_index_buffer(const std::shared_ptr<hazel::renderer::IndexBuffer> &buffer) {
        gl_call(glBindVertexArray(this->rendered_id));
        buffer->bind();
        this->index_buffers.push_back(buffer);
    }
} // namespace hazel::platform::linux