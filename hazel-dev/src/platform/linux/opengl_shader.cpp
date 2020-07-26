#include <hazel/platform/linux/opengl_shader.hpp>

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

namespace hazel::platform::linux
{
    static std::unordered_map<std::string, GLenum> source_types = {
        {"vertex", GL_VERTEX_SHADER},
        {"fragment", GL_FRAGMENT_SHADER},
        {"pixel", GL_FRAGMENT_SHADER}};

    std::string Shader::read_file(const std::string &filepath)
    {
        std::ifstream file(filepath, std::ios_base::in);
        ASSERT(file, "can't open file: " << filepath);

        file.seekg(0, std::ios::end);

        std::string result;
        result.resize(file.tellg());

        file.seekg(0, std::ios::beg);
        file.read(result.data(), result.size());
        file.close();

        return result;
    }

    std::unordered_map<GLenum, std::string> Shader::preprocess(const std::string &filepath, const std::string &contents)
    {
        std::unordered_map<GLenum, std::string> sources;

        const char *type_token = "#type";
        size_t pos = contents.find(type_token);
        while (pos != std::string::npos)
        {
            size_t eol = contents.find_first_of("\n\r", pos);
            ASSERT(eol != std::string::npos, "no newline on shader type def: " << filepath);

            // type metadata
            size_t type_pos = pos + strlen(type_token) + 1; // +1 is space
            std::string type = contents.substr(type_pos, eol - type_pos);
            ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "shader type '" << type << "' is unsupported");

            // next line is any characters that's not a whitespace, after current type metadata
            size_t next_line = contents.find_first_not_of("\n\r", eol);

            // determine the "range" between previous shader type and new shader type
            size_t next = contents.find(type_token, next_line);
            size_t end = (next != std::string::npos) ? next - next_line : contents.size();

            sources[source_types[type]] = contents.substr(next_line, end);
            pos = next;
        }

        return sources;
    }

    void Shader::compile(std::unordered_map<GLenum, std::string> &sources)
    {
        // https://www.khronos.org/opengl/wiki/Shader_Compilation

        this->renderer_id = glCreateProgram();
        HAZEL_DEV_LOG_TRACE("Creating shader (id: {})", this->renderer_id);

        for (auto &kv : sources)
        {
            GLenum type = kv.first;
            const std::string &content = kv.second;

            GLuint shader = glCreateShader(type);
            const GLchar *source = (const GLchar *)content.c_str();
            glShaderSource(shader, 1, &source, 0);
            glCompileShader(shader);

            GLint compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (compiled == GL_FALSE)
            {
                GLint length = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
                std::vector<GLchar> log(length);
                glGetShaderInfoLog(shader, length, &length, &log[0]);
                glDeleteShader(shader);
                HAZEL_DEV_LOG_ERROR("[{}] Shader compilation error: {}", type, log.data());
                assert(false);
            }
            glAttachShader(this->renderer_id, shader);
        }

        glLinkProgram(this->renderer_id);

        GLint linked = 0;
        glGetProgramiv(this->renderer_id, GL_LINK_STATUS, (int *)&linked);
        if (linked == GL_FALSE)
        {
            GLint length = 0;
            glGetProgramiv(this->renderer_id, GL_INFO_LOG_LENGTH, &length);
            std::vector<GLchar> log(length);
            glGetProgramInfoLog(this->renderer_id, length, &length, &log[0]);
            glDeleteProgram(this->renderer_id);
            for (auto &kv : sources)
            {
                glDeleteShader(kv.first);
            }
            HAZEL_DEV_LOG_ERROR("Program link error: {}", log.data());
            assert(false);
        }

        for (auto &kv : sources)
        {
            glDetachShader(this->renderer_id, kv.first);
        }
    }

    std::string Shader::to_string() const
    {
        return std::to_string(this->renderer_id);
    }

    Shader::Shader(const std::string &filepath)
    {
        std::string contents = this->read_file(filepath);
        auto sources = this->preprocess(filepath, contents);
        this->compile(sources);
    }

    Shader::Shader(const std::string &vertex_source, const std::string &fragment_source)
    {
        std::unordered_map<GLenum, std::string> sources = {
            {GL_VERTEX_SHADER, vertex_source},
            {GL_FRAGMENT_SHADER, fragment_source}};
        this->compile(sources);
    }

    Shader::~Shader()
    {
        glDeleteProgram(this->renderer_id);
    }

    void Shader::bind() const
    {
        glUseProgram(this->renderer_id);
    }

    void Shader::unbind() const
    {
        glUseProgram(0);
    }

    void Shader::upload_uniform(const std::string &name, const int &i)
    {
        GLint location = glGetUniformLocation(this->renderer_id, name.c_str());
        ASSERT(location != -1, "uniform doesn't exists: " << name);
        glUniform1i(location, i);
    }

    void Shader::upload_uniform(const std::string &name, const glm::vec3 &vec)
    {
        GLint location = glGetUniformLocation(this->renderer_id, name.c_str());
        ASSERT(location != -1, "uniform doesn't exists: " << name);
        glUniform3f(location, vec.x, vec.y, vec.z);
    }

    void Shader::upload_uniform(const std::string &name, const glm::vec4 &vec)
    {
        GLint location = glGetUniformLocation(this->renderer_id, name.c_str());
        ASSERT(location != -1, "uniform doesn't exists: " << name);
        glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
    }

    void Shader::upload_uniform(const std::string &name, const glm::mat4 &mat)
    {
        GLint location = glGetUniformLocation(this->renderer_id, name.c_str());
        ASSERT(location != -1, "uniform doesn't exists: " << name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
} // namespace hazel::platform::linux
