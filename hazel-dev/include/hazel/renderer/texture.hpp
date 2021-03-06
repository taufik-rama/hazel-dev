#pragma once

#include <hazel/core/core.hpp>

namespace hazel::renderer {
class Texture {
public:
  virtual unsigned int get_width() const = 0;

  virtual unsigned int get_height() const = 0;

  virtual void bind(unsigned int = 0) const = 0;

  virtual void unbind(unsigned int = 0) const = 0;
};

class Texture2D : public Texture {
public:
  static hazel::core::Ref<Texture2D> create(const std::string &);
  static hazel::core::Ref<Texture2D> create(void *data, unsigned int width,
                                            unsigned int height);
};
} // namespace hazel::renderer
