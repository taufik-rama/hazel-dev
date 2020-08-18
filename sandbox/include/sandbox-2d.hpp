#pragma once

#include <hazel-api.hpp>

class Sandbox2D : public hazel::layer::Layer {
public:
  Sandbox2D();

  ~Sandbox2D();

  virtual void on_attach() override;

  virtual void on_detach() override;

  virtual void on_update(hazel::core::Timestep) override;

  virtual void on_event(hazel::event::Event &) override;

  virtual void on_imgui_render() override;

private:
  glm::vec4 square_array = {0.2f, 0.3f, 0.8f, 1.0f};

  hazel::camera::OrthographicController camera_controller;
  hazel::core::Ref<hazel::renderer::Texture2D> checkerboard_texture;
};
