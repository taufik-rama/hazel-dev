#include <hazel/camera/orthographic_controller.hpp>

#include <hazel/input/input.hpp>

namespace hazel::camera {
OrthographicController::OrthographicController(float aspect_ratio,
                                               bool rotation)
    : aspect_ratio(aspect_ratio),
      camera(
          // `zoom_level` is used as a marker so that the
          // matrices values scales.
          // When the zoom level is near, it means the projection value
          // is low, hence lower "screen size" / larger object
          (-this->aspect_ratio * this->zoom_level),
          (this->aspect_ratio * this->zoom_level), -this->zoom_level,
          this->zoom_level),
      rotation(rotation) {}

Orthographic &OrthographicController::get_camera() { return this->camera; }

void OrthographicController::on_update(hazel::core::Timestep ts) {
  TIMER_SCOPE();
  if (hazel::input::Input::is_key_pressed(HAZEL_KEY_W)) {
    this->camera_pos.y += this->camera_pos_speed * ts;
  } else if (hazel::input::Input::is_key_pressed(HAZEL_KEY_S)) {
    this->camera_pos.y -= this->camera_pos_speed * ts;
  }

  if (hazel::input::Input::is_key_pressed(HAZEL_KEY_A)) {
    this->camera_pos.x -= this->camera_pos_speed * ts;
  } else if (hazel::input::Input::is_key_pressed(HAZEL_KEY_D)) {
    this->camera_pos.x += this->camera_pos_speed * ts;
  }

  if (this->rotation) {
    if (hazel::input::Input::is_key_pressed(HAZEL_KEY_Q)) {
      this->camera_rot += this->camera_rot_speed * ts;
    } else if (hazel::input::Input::is_key_pressed(HAZEL_KEY_E)) {
      this->camera_rot -= this->camera_rot_speed * ts;
    }

    this->camera.set_rotation(this->camera_rot);
  }

  this->camera.set_position(this->camera_pos);

  // Slow movements on near zoom level,
  // fast movements on far zoom level
  this->camera_pos_speed = this->zoom_level;
}

void OrthographicController::on_event(hazel::event::Event &e) {
  TIMER_SCOPE();
  hazel::event::EventDispatcher dispatcher(e);
  dispatcher.dispatch<hazel::event::MouseScrollEvent>(
      EVENT_BIND_METHOD_1(OrthographicController::on_mouse_scroll));
  dispatcher.dispatch<hazel::event::WindowResizeEvent>(
      EVENT_BIND_METHOD_1(OrthographicController::on_window_resize));
}

bool OrthographicController::on_mouse_scroll(
    hazel::event::MouseScrollEvent &e) {
  this->zoom_level -= e.get_y_offset() * 0.25f;

  // clamp, we don't want zoom too near/far
  this->zoom_level = std::max(this->zoom_level, 0.25f);
  this->zoom_level = std::min(this->zoom_level, 6.0f);

  this->camera.set_projection((-this->aspect_ratio * this->zoom_level),
                              (this->aspect_ratio * this->zoom_level),
                              -this->zoom_level, this->zoom_level);
  return false;
}

bool OrthographicController::on_window_resize(
    hazel::event::WindowResizeEvent &e) {
  this->aspect_ratio = (float)e.get_width() / (float)e.get_height();
  this->camera.set_projection((-this->aspect_ratio * this->zoom_level),
                              (this->aspect_ratio * this->zoom_level),
                              -this->zoom_level, this->zoom_level);
  return false;
}
} // namespace hazel::camera
