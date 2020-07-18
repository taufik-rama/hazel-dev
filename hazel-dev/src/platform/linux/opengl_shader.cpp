#include <hazel/platform/linux/opengl_shader.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace hazel::platform::linux
{
    std::string Shader::to_string() const
    {
        return std::to_string(this->renderer_id);
    }

    Shader::Shader(std::string &vertex_source, std::string &fragment_source)
    {
        // https://www.khronos.org/opengl/wiki/Shader_Compilation

        // Create an empty vertex shader handle
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar *source = (const GLchar *)vertex_source.c_str();
        glShaderSource(vertexShader, 1, &source, 0);

        // Compile the vertex shader
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(vertexShader);

            HAZEL_DEV_LOG_ERROR("Vertex shader compilation error: {}", infoLog.data());
            assert(false);
        }

        // Create an empty fragment shader handle
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = (const GLchar *)fragment_source.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        // Compile the fragment shader
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(fragmentShader);
            // Either of them. Don't leak shaders.
            glDeleteShader(vertexShader);

            HAZEL_DEV_LOG_ERROR("Fragment shader compilation error: {}", infoLog.data());
            assert(false);
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        this->renderer_id = glCreateProgram();

        HAZEL_DEV_LOG_TRACE("Creating shader (id: {})", this->renderer_id);

        // Attach our shaders to our program
        glAttachShader(this->renderer_id, vertexShader);
        glAttachShader(this->renderer_id, fragmentShader);

        // Link our program
        glLinkProgram(this->renderer_id);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(this->renderer_id, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(this->renderer_id, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(this->renderer_id, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(this->renderer_id);
            // Don't leak shaders either.
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            HAZEL_DEV_LOG_ERROR("Program link error: {}", infoLog.data());
            assert(false);
        }

        // Always detach shaders after a successful link.
        glDetachShader(this->renderer_id, vertexShader);
        glDetachShader(this->renderer_id, fragmentShader);
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
