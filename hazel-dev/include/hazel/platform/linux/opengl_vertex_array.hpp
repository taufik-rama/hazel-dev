#pragma once

#include <hazel/renderer/vertex_array.hpp>

namespace hazel::platform::linux
{
    class VertexArray : public hazel::renderer::VertexArray
    {
    public:
        VertexArray();

        virtual ~VertexArray();

        virtual void bind() const override;

        virtual void unbind() const override;

        virtual void add_vertex_buffer(const std::shared_ptr<hazel::renderer::VertexBuffer> &buffer) override;

        virtual const std::vector<std::shared_ptr<hazel::renderer::VertexBuffer>> &get_vertex_buffer() const override;

        virtual void set_index_buffer(const std::shared_ptr<hazel::renderer::IndexBuffer> &buffer) override;

        virtual const std::shared_ptr<hazel::renderer::IndexBuffer> &get_index_buffer() const override;

    private:
        unsigned int renderer_id;
        std::vector<std::shared_ptr<hazel::renderer::VertexBuffer>> vertex_buffers;
        std::shared_ptr<hazel::renderer::IndexBuffer> index_buffer;
    };
} // namespace hazel::platform::linux
