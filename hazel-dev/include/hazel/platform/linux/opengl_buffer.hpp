#pragma once

#include <hazel/core/core.hpp>
#include <hazel/renderer/buffer.hpp>

#include <glad/glad.h>

namespace hazel::platform::linux {
class VertexBuffer : public hazel::renderer::VertexBuffer {
public:
  VertexBuffer(float *vertices, size_t size);

  virtual ~VertexBuffer();

  virtual std::string to_string() const override;

  virtual void bind() const override;

  virtual void unbind() const override;

  virtual void set_layout(const hazel::renderer::BufferLayout &layout) override;

  virtual const hazel::renderer::BufferLayout &get_layout() const override;

private:
  unsigned int renderer_id;
  hazel::renderer::BufferLayout layout;
};

class IndexBuffer : public hazel::renderer::IndexBuffer {
public:
  IndexBuffer(unsigned int *indices, unsigned int count);

  virtual ~IndexBuffer();

  virtual std::string to_string() const override;

  virtual void bind() const override;

  virtual void unbind() const override;

  virtual unsigned int get_count() const override { return this->count; }

private:
  unsigned int renderer_id;
  unsigned int count;
};
} // namespace hazel::platform::linux