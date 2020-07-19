#pragma once

#include <hazel/renderer/buffer.hpp>

namespace hazel::renderer
{
    class VertexArray
    {
    public:
        virtual void bind() const = 0;

        virtual void unbind() const = 0;

        virtual void add_vertex_buffer(const hazel::Ref<VertexBuffer> &buffer) = 0;

        virtual const std::vector<hazel::Ref<VertexBuffer>> &get_vertex_buffer() const = 0;

        virtual void set_index_buffer(const hazel::Ref<IndexBuffer> &buffer) = 0;

        virtual const hazel::Ref<IndexBuffer> &get_index_buffer() const = 0;

        static VertexArray *create();
    };
} // namespace hazel::renderer
