#pragma once

#include <hazel/renderer/buffer.hpp>

namespace hazel::renderer {
class VertexArray {
public:
  virtual void bind() const = 0;

  virtual void unbind() const = 0;

  virtual void
  add_vertex_buffer(const hazel::core::Ref<VertexBuffer> &buffer) = 0;

  virtual const std::vector<hazel::core::Ref<VertexBuffer>> &
  get_vertex_buffer() const = 0;

  virtual void
  set_index_buffer(const hazel::core::Ref<IndexBuffer> &buffer) = 0;

  virtual const hazel::core::Ref<IndexBuffer> &get_index_buffer() const = 0;

  static hazel::core::Ref<VertexArray> create();
};
} // namespace hazel::renderer
