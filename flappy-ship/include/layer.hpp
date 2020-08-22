#pragma once

#include <hazel-api.hpp>

#include <level.hpp>

#include <imgui.h>

class AppLayer : public hazel::layer::Layer {
public:
  AppLayer();
  ~AppLayer();
  virtual void on_attach() override;
  virtual void on_detach() override;
  virtual void on_update(hazel::core::Timestep ts) override;
  virtual void on_event(hazel::event::Event &e) override;
  virtual void on_imgui_render() override;

private:
  void init_camera();
  bool on_mouse_button_release(hazel::event::MouseButtonReleaseEvent &);

  hazel::core::Scope<hazel::camera::Orthographic> camera;
  Level level;

  ImFont *font;
  enum class GameState {
    PLAY = 0,
    MAIN_MENU = 1,
    GAME_OVER = 2
  } game_state = GameState::MAIN_MENU;
};
