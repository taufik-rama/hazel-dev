#include <hazel/platform/linux/opengl_shader.hpp>

#include <hazel/platform/linux/opengl_log.hpp>

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

namespace hazel::platform::linux {
static std::unordered_map<std::string, GLenum> source_types = {
    {"vertex", GL_VERTEX_SHADER},
    {"fragment", GL_FRAGMENT_SHADER},
    {"pixel", GL_FRAGMENT_SHADER}};

std::string Shader::read_file(const std::string &filepath) {
  TIMER_SCOPE();
  std::ifstream file(filepath, std::ios_base::in | std::ios_base::binary);
  ASSERT(file, "can't open file: " << filepath);

  file.seekg(0, std::ios::end);

  std::string result;
  result.resize(file.tellg());

  file.seekg(0, std::ios::beg);
  file.read(result.data(), result.size());
  file.close();

  return result;
}

std::unordered_map<GLenum, std::string>
Shader::preprocess([[maybe_unused]] const std::string &filepath,
                   const std::string &contents) {
  TIMER_SCOPE();
  std::unordered_map<GLenum, std::string> sources;

  const char *type_token = "#type";
  size_t pos = contents.find(type_token);
  while (pos != std::string::npos) {
    size_t eol = contents.find_first_of("\n\r", pos);
    ASSERT(eol != std::string::npos,
           "no newline on shader type def: " << filepath);

    // type metadata
    size_t type_pos = pos + strlen(type_token) + 1; // +1 is space
    std::string type = contents.substr(type_pos, eol - type_pos);
    ASSERT(type == "vertex" || type == "fragment" || type == "pixel",
           "shader type '" << type << "' is unsupported");

    // next line is any characters that's not a whitespace, after current type
    // metadata
    size_t next_line = contents.find_first_not_of("\n\r", eol);

    // determine the "range" between previous shader type and new shader type
    size_t next = contents.find(type_token, next_line);
    size_t end =
        (next != std::string::npos) ? next - next_line : contents.size();

    sources[source_types[type]] = contents.substr(next_line, end);
    pos = next;
  }

  return sources;
}

void Shader::compile(std::unordered_map<GLenum, std::string> &sources) {
  TIMER_SCOPE();
  // https://www.khronos.org/opengl/wiki/Shader_Compilation

  this->renderer_id = glCreateProgram();
  HAZEL_DEV_LOG_TRACE("Creating shader (id: {})", this->renderer_id);

  for (auto &kv : sources) {
    GLenum type = kv.first;
    const std::string &content = kv.second;

    GLuint shader = glCreateShader(type);
    const GLchar *source = (const GLchar *)content.c_str();
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);

    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
      GLint length = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
      std::vector<GLchar> log(length);
      glGetShaderInfoLog(shader, length, &length, &log[0]);
      glDeleteShader(shader);
      HAZEL_DEV_LOG_ERROR("[{}] Shader compilation error: {}", type,
                          log.data());
      assert(false);
    }
    glAttachShader(this->renderer_id, shader);
  }

  glLinkProgram(this->renderer_id);

  GLint linked = 0;
  glGetProgramiv(this->renderer_id, GL_LINK_STATUS, (int *)&linked);
  if (linked == GL_FALSE) {
    GLint length = 0;
    glGetProgramiv(this->renderer_id, GL_INFO_LOG_LENGTH, &length);
    std::vector<GLchar> log(length);
    glGetProgramInfoLog(this->renderer_id, length, &length, &log[0]);
    glDeleteProgram(this->renderer_id);
    for (auto &kv : sources) {
      glDeleteShader(kv.first);
    }
    HAZEL_DEV_LOG_ERROR("Program link error: {}", log.data());
    assert(false);
  }

  for (auto &kv : sources) {
    glDetachShader(this->renderer_id, kv.first);
  }
}

std::string Shader::to_string() const {
  return std::to_string(this->renderer_id);
}

Shader::Shader(const std::string &filepath) {
  {
    // extract name
    auto last_slash = filepath.find_last_of("/\\");
    last_slash = (last_slash == std::string::npos) ? 0 : last_slash + 1;

    auto last_dot = filepath.rfind('.');
    auto name_count = (last_dot == std::string::npos)
                          ? filepath.size() - last_slash
                          : last_dot - last_slash;

    this->name = filepath.substr(last_slash, name_count);
  }

  std::string contents = this->read_file(filepath);
  auto sources = this->preprocess(filepath, contents);
  this->compile(sources);
}

Shader::Shader(const std::string &name, const std::string &filepath)
    : name(name) {
  std::string contents = this->read_file(filepath);
  auto sources = this->preprocess(filepath, contents);
  this->compile(sources);
}

Shader::Shader(const std::string &name, const std::string &vertex_source,
               const std::string &fragment_source)
    : name(name) {
  std::unordered_map<GLenum, std::string> sources = {
      {GL_VERTEX_SHADER, vertex_source}, {GL_FRAGMENT_SHADER, fragment_source}};
  this->compile(sources);
}

Shader::~Shader() {
  TIMER_SCOPE();
  glDeleteProgram(this->renderer_id);
}

void Shader::bind() const {
  TIMER_SCOPE();
  glUseProgram(this->renderer_id);
}

void Shader::unbind() const {
  TIMER_SCOPE();
  glUseProgram(0);
}

const std::string &Shader::get_name() const { return this->name; }

void Shader::set_uniform(const std::string &name, const int &i) {
  this->upload_uniform(name, i);
}

void Shader::set_uniform(const std::string &name, const glm::vec3 &vec) {
  this->upload_uniform(name, vec);
}

void Shader::set_uniform(const std::string &name, const glm::vec4 &vec) {
  this->upload_uniform(name, vec);
}

void Shader::set_uniform(const std::string &name, const glm::mat4 &mat) {
  this->upload_uniform(name, mat);
}

void Shader::upload_uniform(const std::string &name, const int &i) {
  TIMER_SCOPE();
  GLint location = glGetUniformLocation(this->renderer_id, name.c_str());
  location == -1 ? HAZEL_DEV_LOG_WARN("uniform doesn't exists: {}", name)
                 : (void)0;
  gl_call(glUniform1i(location, i));
}

void Shader::upload_uniform(const std::string &name, const glm::vec3 &vec) {
  TIMER_SCOPE();
  GLint location = glGetUniformLocation(this->renderer_id, name.c_str());
  location == -1 ? HAZEL_DEV_LOG_WARN("uniform doesn't exists: {}", name)
                 : (void)0;
  gl_call(glUniform3f(location, vec.x, vec.y, vec.z));
}

void Shader::upload_uniform(const std::string &name, const glm::vec4 &vec) {
  TIMER_SCOPE();
  GLint location = glGetUniformLocation(this->renderer_id, name.c_str());
  location == -1 ? HAZEL_DEV_LOG_WARN("uniform doesn't exists: {}", name)
                 : (void)0;
  gl_call(glUniform4f(location, vec.x, vec.y, vec.z, vec.w));
}

void Shader::upload_uniform(const std::string &name, const glm::mat4 &mat) {
  TIMER_SCOPE();
  GLint location = glGetUniformLocation(this->renderer_id, name.c_str());
  location == -1 ? HAZEL_DEV_LOG_WARN("uniform doesn't exists: {}", name)
                 : (void)0;
  gl_call(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)));
}
} // namespace hazel::platform::linux
