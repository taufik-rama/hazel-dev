#pragma once

#include <hazel/core/core.hpp>
#include <hazel/core/window.hpp>
#include <hazel/event/window.hpp>
#include <hazel/layer/_fwd.hpp>

namespace hazel::core {
class Application {
public:
  Application();

  virtual ~Application();

  void run();

  void event_callback(hazel::event::Event &);

  void add_layer(hazel::layer::Layer *);

  void add_layer_overlay(hazel::layer::Layer *);

  static Application *get_application() { return instance; }

  Window &get_window() { return *this->window; };

private:
  bool window_close_event_callback(hazel::event::WindowCloseEvent &);

  bool window_resize_event_callback(hazel::event::WindowResizeEvent &);

  bool window_minimize_event_callback(hazel::event::WindowMinimizeEvent &);

  Scope<Window> window;
  hazel::layer::Collection *layers;
  hazel::layer::ImGui *imgui;

  static Application *instance;
  bool is_running = true;
  bool is_minimized = false;
  float last_frame_duration = 0.0f;
};
} // namespace hazel::core
