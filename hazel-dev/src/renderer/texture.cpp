#include <hazel/renderer/texture.hpp>

#include <hazel/platform/linux/opengl_texture.hpp>
#include <hazel/renderer/library.hpp>

namespace hazel::renderer {
hazel::core::Ref<Texture2D> Texture2D::create(const std::string &path) {
  switch (Library::get_api_library()) {
  case Library::API::NONE:
    break;

  case Library::API::OPENGL:
    return hazel::core::create_ref<hazel::platform::linux::Texture2D>(path);
  }
  assert(false);
}

hazel::core::Ref<Texture2D> Texture2D::create(void *data, unsigned int width,
                                              unsigned int height) {
  switch (Library::get_api_library()) {
  case Library::API::NONE:
    break;

  case Library::API::OPENGL:
    return hazel::core::create_ref<hazel::platform::linux::Texture2D>(
        data, width, height);
  }
  assert(false);
}
} // namespace hazel::renderer
