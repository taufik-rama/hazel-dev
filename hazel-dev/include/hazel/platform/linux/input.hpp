#pragma once

#include <hazel/input/input.hpp>

#include <GLFW/glfw3.h>

namespace hazel::platform::linux
{
    class Input : public hazel::input::Input
    {
    protected:
        virtual bool is_key_pressed_impl(unsigned int key_code) override;

        virtual bool is_mouse_button_pressed_impl(unsigned int button) override;

        virtual std::pair<float, float> get_mouse_pos_impl() override;
    };
} // namespace hazel::platform::linux
