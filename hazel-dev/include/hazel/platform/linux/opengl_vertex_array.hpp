#pragma once

#include <hazel/renderer/vertex_array.hpp>

namespace hazel::platform::linux {
class VertexArray : public hazel::renderer::VertexArray {
public:
  VertexArray();

  virtual ~VertexArray();

  virtual void bind() const override;

  virtual void unbind() const override;

  virtual void add_vertex_buffer(
      const hazel::core::Ref<hazel::renderer::VertexBuffer> &buffer) override;

  virtual const std::vector<hazel::core::Ref<hazel::renderer::VertexBuffer>> &
  get_vertex_buffer() const override;

  virtual void set_index_buffer(
      const hazel::core::Ref<hazel::renderer::IndexBuffer> &buffer) override;

  virtual const hazel::core::Ref<hazel::renderer::IndexBuffer> &
  get_index_buffer() const override;

private:
  unsigned int renderer_id;
  std::vector<hazel::core::Ref<hazel::renderer::VertexBuffer>> vertex_buffers;
  hazel::core::Ref<hazel::renderer::IndexBuffer> index_buffer;
};
} // namespace hazel::platform::linux
