#include <hazel/renderer/shader.hpp>

#include <hazel/platform/linux/opengl_shader.hpp>
#include <hazel/renderer/library.hpp>

// Shader
namespace hazel::renderer {
hazel::core::Ref<Shader> Shader::create(const std::string &filepath) {
  switch (Library::get_api_library()) {
  case Library::API::NONE:
    break;

  case Library::API::OPENGL:
    return hazel::core::create_ref<hazel::platform::linux::Shader>(filepath);
  }
  assert(false);
}

hazel::core::Ref<Shader> Shader::create(const std::string &name,
                                        const std::string &filepath) {
  switch (Library::get_api_library()) {
  case Library::API::NONE:
    break;

  case Library::API::OPENGL:
    return hazel::core::create_ref<hazel::platform::linux::Shader>(name,
                                                                   filepath);
  }
  assert(false);
}

hazel::core::Ref<Shader> Shader::create(const std::string &name,
                                        const std::string &vertex_source,
                                        const std::string &fragment_source) {
  switch (Library::get_api_library()) {
  case Library::API::NONE:
    break;

  case Library::API::OPENGL:
    return hazel::core::create_ref<hazel::platform::linux::Shader>(
        name, vertex_source, fragment_source);
  }
  assert(false);
}
} // namespace hazel::renderer

// ShaderArray
namespace hazel::renderer {
void ShaderArray::add(const hazel::core::Ref<Shader> &shader) {
  this->add(shader->get_name(), shader);
}

void ShaderArray::add(const std::string &name,
                      const hazel::core::Ref<Shader> &shader) {
  ASSERT(!this->exists(name), "duplicated shader: " << name);
  this->shaders[name] = shader;
}

hazel::core::Ref<Shader> ShaderArray::load(const std::string &filepath) {
  auto shader = Shader::create(filepath);
  this->add(shader);
  return shader;
}

hazel::core::Ref<Shader> ShaderArray::load(const std::string &name,
                                           const std::string &filepath) {
  auto shader = Shader::create(name, filepath);
  this->add(name, shader);
  return shader;
}

hazel::core::Ref<Shader> ShaderArray::load(const std::string &name,
                                           const std::string &vertex_source,
                                           const std::string &fragment_source) {
  auto shader = Shader::create(name, vertex_source, fragment_source);
  this->add(name, shader);
  return shader;
}

hazel::core::Ref<Shader> ShaderArray::get(const std::string &name) {
  ASSERT(this->exists(name), "shader does not exists: " << name);
  return this->shaders[name];
}

bool ShaderArray::exists(const std::string &name) {
  auto got = this->shaders.find(name);
  return got != this->shaders.end();
}
} // namespace hazel::renderer
