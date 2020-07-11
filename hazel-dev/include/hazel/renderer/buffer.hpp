#pragma once

#include <hazel/renderer/shader.hpp>

namespace hazel::renderer
{
    struct BufferLayoutElement
    {
        std::string name;
        intptr_t offset;
        unsigned int size;
        ShaderDataType type;
        bool normalized;

        BufferLayoutElement(ShaderDataType type, const std::string name, bool normalized = false);

        unsigned int get_component_count() const;
    };

    class BufferLayout
    {
    public:
        BufferLayout(const std::initializer_list<BufferLayoutElement> &e);

        const std::vector<BufferLayoutElement> &get_elements() const { return this->elements; }

        const unsigned int &get_stride() const { return this->stride; }

        std::vector<BufferLayoutElement>::const_iterator begin() const { return this->elements.begin(); }

        std::vector<BufferLayoutElement>::const_iterator end() const { return this->elements.end(); }

    private:
        std::vector<BufferLayoutElement> elements;

        unsigned int stride = 0;
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() {}

        virtual void bind() const = 0;

        virtual void unbind() const = 0;

        virtual void set_layout(const BufferLayout &layout) = 0;

        virtual const BufferLayout &get_layout() const = 0;

        static VertexBuffer *create(float *vertices, size_t size);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() {}

        virtual void bind() const = 0;

        virtual void unbind() const = 0;

        virtual unsigned int get_count() const = 0;

        static IndexBuffer *create(unsigned int *indices, size_t size);
    };
} // namespace hazel::renderer