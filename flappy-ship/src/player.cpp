#include <player.hpp>

static float GRAVITY = 0.2f;

Player::Player() {}
Player::~Player() {}

void Player::load_assets() {
  this->player_texture =
      hazel::renderer::Texture2D::create("../assets/textures/Ship.png");
}

void Player::on_update(hazel::core::Timestep ts) {
  this->time += ts;

  if (hazel::input::Input::is_key_pressed(HAZEL_KEY_SPACE)) {
    this->velocity.y += GRAVITY;
  } else {
    this->velocity.y -= GRAVITY;
  }

  this->velocity.y = glm::clamp(this->velocity.y, -100.0f, 100.0f);
  this->position += this->velocity * (float)ts;
}

void Player::on_render() {
  hazel::renderer::Renderer2D::draw_quad(
      {this->position.x, this->position.y, 1.0f}, 270.0f, {1.0f, 1.0f},
      this->player_texture);
}

void Player::reset() {
  this->position = DEFAULT_PLAYER_POSITION;
  this->velocity = DEFAULT_PLAYER_VELOCITY;
}