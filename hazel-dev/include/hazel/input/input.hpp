#pragma once

#include <hazel/core/core.hpp>
#include <hazel/input/key_codes.hpp>
#include <hazel/input/mouse_button_codes.hpp>

namespace hazel::input {
class Input {
public:
  static bool is_key_pressed(unsigned int key_code) {
    return instance->is_key_pressed_impl(key_code);
  }

  static bool is_mouse_button_pressed(unsigned int button) {
    return instance->is_mouse_button_pressed_impl(button);
  }

  static std::pair<float, float> get_mouse_pos() {
    return instance->get_mouse_pos_impl();
  }

  static void set_instance(Input *i) { instance = i; }

protected:
  virtual bool is_key_pressed_impl(unsigned int) = 0;

  virtual bool is_mouse_button_pressed_impl(unsigned int) = 0;

  virtual std::pair<float, float> get_mouse_pos_impl() = 0;

private:
  static Input *instance;
};
} // namespace hazel::input
