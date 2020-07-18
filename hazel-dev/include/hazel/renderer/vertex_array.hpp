#pragma once

#include <hazel/renderer/buffer.hpp>

namespace hazel::renderer
{
    class VertexArray
    {
    public:
        virtual void bind() const = 0;

        virtual void unbind() const = 0;

        virtual void add_vertex_buffer(const std::shared_ptr<VertexBuffer> &buffer) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>> &get_vertex_buffer() const = 0;

        virtual void set_index_buffer(const std::shared_ptr<IndexBuffer> &buffer) = 0;

        virtual const std::shared_ptr<IndexBuffer> &get_index_buffer() const = 0;

        static VertexArray *create();
    };
} // namespace hazel::renderer
