#pragma once

#include <hazel-api.hpp>

#include <player.hpp>

struct Obstacle {
  glm::vec3 pos;
  float rot;
  glm::vec2 size;
};

class Level {
public:
  void init();
  void on_update(hazel::core::Timestep ts);
  void on_render();
  Player &get_player_ref() { return this->player; }
  bool is_game_over();
  void reset();

private:
  bool test_player_collision();

  Player player;
  hazel::core::Ref<hazel::renderer::Texture2D> obstacles_texture;
  std::vector<Obstacle> obstacles;
  bool game_over = false;
};