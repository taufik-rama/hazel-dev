#include <level.hpp>

static void create_obstacle(Obstacle &obs, unsigned int index) {
  float y_axis;
  if (index % 2 == 0) {
    y_axis = -12.0f;
    obs.rot = 0.0f;
  } else {
    y_axis = 12.0f;
    obs.rot = 180.0f;
  }
  obs.pos = {(float)(index * 12), y_axis, 0.0f};
  obs.size = {8.0f, 8.0f};
}

void Level::init() {
  this->obstacles_texture =
      hazel::renderer::Texture2D::create("../assets/textures/Triangle.png");
  this->player.load_assets();
  this->obstacles.resize(6);
  for (unsigned int i = 0; i < this->obstacles.size(); i++) {
    create_obstacle(this->obstacles[i], i);
  }
}

void Level::on_update(hazel::core::Timestep ts) {
  if (this->test_player_collision()) {
    this->game_over = true;
  }
  this->player.on_update(ts);
}

void Level::on_render() {

  const auto &player_pos = this->player.get_position();

  // Floor & ceiling
  hazel::renderer::Renderer2D::draw_quad(
      {player_pos.x, 25.0f}, 0.0f, {60.0f, 7.5f}, {0.8f, 0.2f, 0.3f, 1.0f});
  hazel::renderer::Renderer2D::draw_quad(
      {player_pos.x, -25.0f}, 0.0f, {60.0f, 7.5f}, {0.8f, 0.2f, 0.3f, 1.0f});

  // Obstacles
  for (const auto &obstacle : this->obstacles) {
    hazel::renderer::Renderer2D::draw_quad(
        obstacle.pos, obstacle.rot, obstacle.size, this->obstacles_texture);
  }

  this->player.on_render();
}

bool Level::is_game_over() { return this->game_over; }

void Level::reset() {
  this->game_over = false;
  this->player.reset();
}

bool Level::test_player_collision() {
  if (glm::abs(this->player.get_position().y) > 17.0f) {
    return true;
  }
  return false;
}
