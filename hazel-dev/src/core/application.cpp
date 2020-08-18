#include <hazel/core/application.hpp>

#include <hazel/core/timestep.hpp>
#include <hazel/layer/collection.hpp>
#include <hazel/layer/imgui.hpp>
#include <hazel/renderer/renderer.hpp>
#include <hazel/renderer/renderer2D.hpp>

// `glfwGetTime`
#include <GLFW/glfw3.h>

namespace hazel::core {
Application *Application::instance = nullptr;

Application::Application() {
  TIMER_SCOPE();
  assert(!Application::instance);
  Application::instance = this;

  this->layers = new hazel::layer::Collection();

  this->window = Scope<Window>(Window::create());
  this->window->set_event_callback(
      EVENT_BIND_METHOD_1(Application::event_callback));

  hazel::renderer::Renderer::init();
  hazel::renderer::Renderer2D::init();

  this->imgui = new hazel::layer::ImGui();
  this->add_layer_overlay(this->imgui);
}

Application::~Application() {
  TIMER_SCOPE();
  hazel::renderer::Renderer::shutdown();
  hazel::renderer::Renderer2D::shutdown();
}

void Application::event_callback(hazel::event::Event &e) {
  hazel::event::EventDispatcher dispatcher(e);
  dispatcher.dispatch<hazel::event::WindowCloseEvent>(
      EVENT_BIND_METHOD_1(Application::window_close_event_callback));
  dispatcher.dispatch<hazel::event::WindowResizeEvent>(
      EVENT_BIND_METHOD_1(Application::window_resize_event_callback));
  dispatcher.dispatch<hazel::event::WindowMinimizeEvent>(
      EVENT_BIND_METHOD_1(Application::window_minimize_event_callback));

  // Handle for each layers
  for (auto it = this->layers->end(); it != this->layers->begin();) {
    --it;
    (*it)->on_event(e);
    if (e.is_handled()) {
      break;
    }
  }
}

bool Application::window_close_event_callback(
    hazel::event::WindowCloseEvent &) {
  this->is_running = false;
  return true;
}

bool Application::window_resize_event_callback(
    hazel::event::WindowResizeEvent &e) {
  hazel::renderer::Renderer::resize_window(e.get_width(), e.get_height());
  return false;
}

bool Application::window_minimize_event_callback(
    hazel::event::WindowMinimizeEvent &e) {
  if (e.is_minimized()) {
    this->is_minimized = true;
    return false;
  }
  this->is_minimized = false;
  return false;
}

void Application::add_layer(hazel::layer::Layer *layer) {
  this->layers->add(layer);
  layer->on_attach();
}

void Application::add_layer_overlay(hazel::layer::Layer *layer) {
  this->layers->add_overlay(layer);
  layer->on_attach();
}

void Application::run() {
  this->is_running = true;
  while (this->is_running) {
    TIMER_SCOPE();
    float time = glfwGetTime();
    hazel::core::Timestep ts(time - this->last_frame_duration);
    this->last_frame_duration = time;

    if (!this->is_minimized) {
      for (auto layer : *this->layers) {
        layer->on_update(ts);
      }
      this->imgui->begin();
      for (auto layer : *this->layers) {
        layer->on_imgui_render();
      }
      this->imgui->end();
    }

    this->window->on_update();
  }
}
} // namespace hazel::core
