#pragma once

#include <hazel/core/core.hpp>
#include <hazel/event/event.hpp>

// ::TODO:: Improve native window return type

namespace hazel::core {
class WindowProps {
public:
  std::string title;
  unsigned short width;
  unsigned short height;

  WindowProps(const std::string &title = "Hazel Dev Engine",
              unsigned short width = 1280, unsigned short height = 720)
      : title(title), width(width), height(height) {}
};

class Window {
public:
  using EventCallbackFn = std::function<void(event::Event &)>;

  virtual ~Window() {}

  virtual void on_update() = 0;

  virtual unsigned short get_width() const = 0;

  virtual unsigned short get_height() const = 0;

  virtual void set_event_callback(const EventCallbackFn &) = 0;

  virtual void set_vsync(bool) = 0;

  virtual bool get_vsync() const = 0;

  static Window *create(const WindowProps &props = WindowProps());

  virtual void *get_native_window() const = 0;

  virtual void set_title(const std::string &) = 0;
};
} // namespace hazel::core
