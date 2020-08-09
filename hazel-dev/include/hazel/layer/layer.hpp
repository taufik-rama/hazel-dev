#pragma once

#include <hazel/core/core.hpp>
#include <hazel/core/timestep.hpp>
#include <hazel/event/event.hpp>

namespace hazel::layer {
class Layer {
public:
  Layer(const std::string &name = "Layer") : debug_name(name) {}

  virtual ~Layer() = default;

  virtual void on_attach() {}

  virtual void on_detach() {}

  virtual void on_update(hazel::core::Timestep) {}

  virtual void on_event(hazel::event::Event &) {}

  virtual void on_imgui_render() {}

  const std::string &to_string() const { return this->debug_name; }

private:
  std::string debug_name;
};
} // namespace hazel::layer
