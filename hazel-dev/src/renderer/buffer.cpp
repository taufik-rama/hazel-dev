#include <hazel/renderer/buffer.hpp>

#include <hazel/core/application.hpp>
#include <hazel/platform/linux/opengl_buffer.hpp>
#include <hazel/renderer/library.hpp>

namespace hazel::renderer {
BufferLayoutElement::BufferLayoutElement(ShaderDataType type,
                                         const std::string name,
                                         bool normalized /* = false */)
    : name(name), offset(0), size(shader_data_type_size(type)), type(type),
      normalized(normalized) {
  this->size = shader_data_type_size(type);
}

unsigned int BufferLayoutElement::get_component_count() const {
  switch (this->type) {
  case ShaderDataType::FLOAT:
    return 1;
  case ShaderDataType::FLOAT2:
    return 2;
  case ShaderDataType::FLOAT3:
    return 3;
  case ShaderDataType::FLOAT4:
    return 4;
  default:
    break;
  }
  assert(false);
}

BufferLayout::BufferLayout(const std::initializer_list<BufferLayoutElement> &e)
    : elements(e) {
  this->stride = 0;
  for (auto &e : this->elements) {
    e.offset = this->stride;
    this->stride += e.size;
  }
}

hazel::core::Ref<VertexBuffer> VertexBuffer::create(float *vertices,
                                                    size_t size) {
  switch (Library::get_api_library()) {
  case Library::API::NONE:
    break;

  case Library::API::OPENGL:
    return std::make_shared<hazel::platform::linux::VertexBuffer>(vertices,
                                                                  size);
  }
  assert(false);
}

hazel::core::Ref<IndexBuffer> IndexBuffer::create(unsigned int *indices,
                                                  size_t size) {
  switch (Library::get_api_library()) {
  case Library::API::NONE:
    break;

  case Library::API::OPENGL:
    return std::make_shared<hazel::platform::linux::IndexBuffer>(indices, size);
  }
  assert(false);
}
} // namespace hazel::renderer
