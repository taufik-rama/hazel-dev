#pragma once

#include <hazel/core/core.hpp>

#include <glm/glm.hpp>

namespace hazel::renderer {
enum class ShaderDataType : unsigned int {
  NONE = 0,
  FLOAT,
  FLOAT2,
  FLOAT3,
  FLOAT4,
};

inline static unsigned int shader_data_type_size(ShaderDataType t) {
  switch (t) {
  case ShaderDataType::NONE:
    return 0;
  case ShaderDataType::FLOAT:
    return 4;
  case ShaderDataType::FLOAT2:
    return 4 * 2;
  case ShaderDataType::FLOAT3:
    return 4 * 3;
  case ShaderDataType::FLOAT4:
    return 4 * 4;
  }
  assert(false);
}

class Shader {
public:
  virtual void bind() const = 0;

  virtual void unbind() const = 0;

  static hazel::core::Ref<Shader> create(const std::string &filepath);

  static hazel::core::Ref<Shader> create(const std::string &name,
                                         const std::string &filepath);

  static hazel::core::Ref<Shader> create(const std::string &name,
                                         const std::string &vertex_source,
                                         const std::string &fragment_source);

  virtual void set_uniform(const std::string &name, const int &i) = 0;

  virtual void set_uniform(const std::string &name, const glm::vec3 &vec) = 0;

  virtual void set_uniform(const std::string &name, const glm::vec4 &vec) = 0;

  virtual void set_uniform(const std::string &name, const glm::mat4 &mat) = 0;

  virtual const std::string &get_name() const = 0;

  virtual std::string to_string() const = 0;
  FORMAT_TO_STRING(Shader)
};

class ShaderArray {
public:
  void add(const hazel::core::Ref<Shader> &shader);

  void add(const std::string &name, const hazel::core::Ref<Shader> &shader);

  hazel::core::Ref<Shader> load(const std::string &filepath);

  hazel::core::Ref<Shader> load(const std::string &name,
                                const std::string &filepath);

  hazel::core::Ref<Shader> load(const std::string &name,
                                const std::string &vertex_source,
                                const std::string &fragment_source);

  hazel::core::Ref<Shader> get(const std::string &name);

  bool exists(const std::string &name);

private:
  std::unordered_map<std::string, hazel::core::Ref<Shader>> shaders;
};
} // namespace hazel::renderer
