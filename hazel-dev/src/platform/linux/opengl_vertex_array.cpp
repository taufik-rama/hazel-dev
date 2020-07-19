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
        gl_call(glCreateVertexArrays(1, &this->renderer_id));
        HAZEL_DEV_LOG_TRACE("Creating vertex array (id: {})", this->renderer_id);
    }

    VertexArray::~VertexArray()
    {
        gl_call(glDeleteVertexArrays(1, &this->renderer_id));
    }

    void VertexArray::bind() const
    {
        gl_call(glBindVertexArray(this->renderer_id));
    }

    void VertexArray::unbind() const
    {
        gl_call(glBindVertexArray(0));
    }

    void VertexArray::add_vertex_buffer(const hazel::Ref<hazel::renderer::VertexBuffer> &buffer)
    {
        HAZEL_DEV_LOG_TRACE("Adding vertex buffer (id: {}) to vertex array (id: {})", *buffer, this->renderer_id);

        if (buffer->get_layout().get_elements().size() == 0)
        {
            HAZEL_DEV_LOG_WARN("An empty vertex buffer layout is added", "");
        }

        gl_call(glBindVertexArray(this->renderer_id));
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
        buffer->unbind();
    }

    const std::vector<hazel::Ref<hazel::renderer::VertexBuffer>> &VertexArray::get_vertex_buffer() const
    {
        return this->vertex_buffers;
    }

    void VertexArray::set_index_buffer(const hazel::Ref<hazel::renderer::IndexBuffer> &buffer)
    {
        HAZEL_DEV_LOG_TRACE("Setting index buffer (id: {}) to vertex array (id: {})", *buffer, this->renderer_id);
        gl_call(glBindVertexArray(this->renderer_id));
        buffer->bind();
        this->index_buffer = buffer;
        buffer->unbind();
    }

    const hazel::Ref<hazel::renderer::IndexBuffer> &VertexArray::get_index_buffer() const
    {
        return this->index_buffer;
    }
} // namespace hazel::platform::linux