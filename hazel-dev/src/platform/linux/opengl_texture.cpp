#include <hazel/platform/linux/opengl_texture.hpp>

#include <hazel/platform/linux/opengl_log.hpp>

#include <glad/glad.h>
#include <stb/image.h>

namespace hazel::platform::linux {
Texture2D::Texture2D(const std::string &path) : path(path) {
  TIMER_SCOPE();
  int width, height, channels;
  stbi_set_flip_vertically_on_load(1); // OpenGL requires invert
  stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  ASSERT(data, "Failed to load texture on: " << path);
  this->width = width;
  this->height = height;

  // `internal_format` is what OpenGL will do to interpret the data
  // `data_format` is what the actual data format that we supply
  GLenum internal_format = 0, data_format = 0;
  if (channels == 4) {
    internal_format = GL_RGBA8;
    data_format = GL_RGBA;
  } else if (channels == 3) {
    internal_format = GL_RGB8;
    data_format = GL_RGB;
  } else {
    ASSERT(false, "unsupported channel: " << channels);
  }

  gl_call(glCreateTextures(GL_TEXTURE_2D, 1, &this->renderer_id));
  gl_call(glTextureStorage2D(this->renderer_id, 1, internal_format, this->width,
                             this->height));
  gl_call(
      glTextureParameteri(this->renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  gl_call(glTextureParameteri(this->renderer_id, GL_TEXTURE_MAG_FILTER,
                              GL_NEAREST));
  gl_call(glTextureParameteri(this->renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT));
  gl_call(glTextureParameteri(this->renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT));
  gl_call(glTextureSubImage2D(this->renderer_id, 0, 0, 0, this->width,
                              this->height, data_format, GL_UNSIGNED_BYTE,
                              data));

  stbi_image_free(data);
}

Texture2D::Texture2D(void *data, unsigned int width, unsigned int height)
    : width(width), height(height) {
  TIMER_SCOPE();
  // `internal_format` is what OpenGL will do to interpret the data
  // `data_format` is what the actual data format that we supply
  GLenum internal_format = GL_RGBA8, data_format = GL_RGBA;

  gl_call(glCreateTextures(GL_TEXTURE_2D, 1, &this->renderer_id));
  gl_call(glTextureStorage2D(this->renderer_id, 1, internal_format, this->width,
                             this->height));
  gl_call(
      glTextureParameteri(this->renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  gl_call(glTextureParameteri(this->renderer_id, GL_TEXTURE_MAG_FILTER,
                              GL_NEAREST));
  gl_call(glTextureParameteri(this->renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT));
  gl_call(glTextureParameteri(this->renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT));
  gl_call(glTextureSubImage2D(this->renderer_id, 0, 0, 0, this->width,
                              this->height, data_format, GL_UNSIGNED_BYTE,
                              data));
}

Texture2D::~Texture2D() {
  TIMER_SCOPE();
  glDeleteTextures(1, &this->renderer_id);
}

unsigned int Texture2D::get_width() const { return this->width; }

unsigned int Texture2D::get_height() const { return this->height; }

void Texture2D::bind(unsigned int slot) const {
  TIMER_SCOPE();
  gl_call(glBindTextureUnit(slot, this->renderer_id));
}

void Texture2D::unbind(unsigned int slot) const {
  TIMER_SCOPE();
  gl_call(glBindTextureUnit(slot, 0));
}
} // namespace hazel::platform::linux
