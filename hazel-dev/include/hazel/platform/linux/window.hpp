#pragma once

#include <hazel/core/window.hpp>
#include <hazel/renderer/graphics_context.hpp>

#include <GLFW/glfw3.h>

namespace hazel::platform::linux {
class Window : public hazel::core::Window {
public:
  Window(const hazel::core::WindowProps &);

  virtual ~Window();

  void on_update() override;

  virtual unsigned short get_width() const override;

  virtual unsigned short get_height() const override;

  virtual void set_event_callback(const EventCallbackFn &) override;

  virtual void set_vsync(bool) override;

  virtual bool get_vsync() const override;

  virtual void *get_native_window() const override { return this->window; };

private:
  GLFWwindow *window;

  hazel::renderer::GraphicsContext *context;

  struct WindowData {
    std::string title;
    unsigned short width, height;
    bool vsync;
    hazel::core::Window::EventCallbackFn event_callback;
  } window_data;

  virtual void init(const hazel::core::WindowProps &);

  virtual void shutdown();
};
} // namespace hazel::platform::linux
