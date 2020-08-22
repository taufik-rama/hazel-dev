#pragma once

#include <hazel-api.hpp>

const glm::vec2 DEFAULT_PLAYER_POSITION = {-10.0f, 0.0f};
const glm::vec2 DEFAULT_PLAYER_VELOCITY = {15.0f, 1.0f};

class Player {
public:
  Player();
  ~Player();
  void load_assets();
  void on_update(hazel::core::Timestep ts);
  void on_render();
  const glm::vec2 &get_position() const { return this->position; }
  void reset();

private:
  hazel::core::Ref<hazel::renderer::Texture2D> player_texture;
  glm::vec2 position = DEFAULT_PLAYER_POSITION;
  glm::vec2 velocity = DEFAULT_PLAYER_VELOCITY;
  float time = 0.0f;
};