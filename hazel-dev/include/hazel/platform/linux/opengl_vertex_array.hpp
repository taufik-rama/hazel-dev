#pragma once

#include <hazel/renderer/vertex_array.hpp>

namespace hazel::platform::linux
{
    class VertexArray : public hazel::renderer::VertexArray
    {
    public:
        VertexArray();

        virtual ~VertexArray() {}

        virtual void bind() const override;

        virtual void unbind() const override;

        virtual void add_vertex_buffer(const std::shared_ptr<hazel::renderer::VertexBuffer> &buffer) override;

        virtual void add_index_buffer(const std::shared_ptr<hazel::renderer::IndexBuffer> &buffer) override;

    private:
        unsigned int rendered_id;
        std::vector<std::shared_ptr<hazel::renderer::VertexBuffer>> vertex_buffers;
        std::vector<std::shared_ptr<hazel::renderer::IndexBuffer>> index_buffers;
    };
} // namespace hazel::platform::linux
