#pragma once

#include <hazel/renderer/shader.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace hazel::platform::linux {
class Shader : public hazel::renderer::Shader {
public:
  Shader(const std::string &filepath);

  Shader(const std::string &name, const std::string &filepath);

  Shader(const std::string &name, const std::string &vertex_source,
         const std::string &fragment_source);

  virtual ~Shader();

  virtual void bind() const override;

  virtual void unbind() const override;

  virtual const std::string &get_name() const override;

  virtual void set_uniform(const std::string &name, const int &i) override;

  virtual void set_uniform(const std::string &name,
                           const glm::vec3 &vec) override;

  virtual void set_uniform(const std::string &name,
                           const glm::vec4 &vec) override;

  virtual void set_uniform(const std::string &name,
                           const glm::mat4 &mat) override;

  void upload_uniform(const std::string &, const int &);

  void upload_uniform(const std::string &, const glm::vec3 &);

  void upload_uniform(const std::string &, const glm::vec4 &);

  void upload_uniform(const std::string &, const glm::mat4 &);

  virtual std::string to_string() const override;

private:
  std::string read_file(const std::string &);

  std::unordered_map<GLenum, std::string> preprocess(const std::string &,
                                                     const std::string &);

  void compile(std::unordered_map<GLenum, std::string> &);

  unsigned int renderer_id;

  std::string name;
};
} // namespace hazel::platform::linux
