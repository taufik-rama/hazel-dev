#include <hazel/platform/linux/opengl_buffer.hpp>

#include <hazel/platform/linux/opengl_log.hpp>

namespace hazel::platform::linux
{
    VertexBuffer::VertexBuffer(float *vertices, size_t size) : layout({})
    {
        gl_call(glCreateBuffers(1, &this->renderer_id));
        gl_call(glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id));
        gl_call(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
    }

    VertexBuffer::~VertexBuffer()
    {
        gl_call(glDeleteBuffers(1, &this->renderer_id));
    }

    void VertexBuffer::bind() const
    {
        gl_call(glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id));
    }

    void VertexBuffer::unbind() const
    {
        gl_call(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void VertexBuffer::set_layout(const hazel::renderer::BufferLayout &layout) { this->layout = layout; }

    const hazel::renderer::BufferLayout &VertexBuffer::get_layout() const { return this->layout; }

    IndexBuffer::IndexBuffer(unsigned int *indices, unsigned int count) : count(count)
    {
        gl_call(glCreateBuffers(1, &this->renderer_id));
        gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer_id));
        gl_call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
    }

    IndexBuffer::~IndexBuffer()
    {
        gl_call(glDeleteBuffers(1, &this->renderer_id));
    }

    void IndexBuffer::bind() const
    {
        gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer_id));
    }

    void IndexBuffer::unbind() const
    {
        gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
} // namespace hazel::platform::linux