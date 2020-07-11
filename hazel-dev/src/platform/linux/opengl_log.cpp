#include <hazel/platform/linux/opengl_log.hpp>

#include <hazel/core.hpp>

#include <glad/glad.h>

void gl_clear_error()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool gl_log(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << file << ":"
                  << line << " on "
                  << function << " [OpenGL error]: "
                  << std::hex << error
                  << std::dec << std::endl;
        return false;
    }
    return true;
}
