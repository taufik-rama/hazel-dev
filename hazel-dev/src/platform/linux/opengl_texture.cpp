#include <hazel/platform/linux/opengl_texture.hpp>

#include <glad/glad.h>
#include <stb/image.h>

namespace hazel::platform::linux
{
    Texture2D::Texture2D(const std::string &path) : path(path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1); // OpenGL requires invert
        stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        ASSERT(data, "Failed to load texture on: " << path);
        this->width = width;
        this->height = height;

        glCreateTextures(GL_TEXTURE_2D, 1, &this->renderer_id);
        glTextureStorage2D(this->renderer_id, 1, GL_RGB8, this->width, this->height);
        glTextureParameteri(this->renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(this->renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureSubImage2D(this->renderer_id, 0, 0, 0, this->width, this->height, GL_RGB, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &this->renderer_id);
    }

    unsigned int Texture2D::get_width() const
    {
        return this->width;
    }

    unsigned int Texture2D::get_height() const
    {
        return this->height;
    }

    void Texture2D::bind(unsigned int slot) const
    {
        glBindTextureUnit(slot, this->renderer_id);
    }

    void Texture2D::unbind(unsigned int slot) const
    {
        glBindTextureUnit(slot, 0);
    }
} // namespace hazel::platform::linux
