#include <hazel/platform/linux/input.hpp>

#include <hazel/application.hpp>

namespace hazel::platform::linux
{
    bool Input::is_key_pressed_impl(unsigned int key_code)
    {
        auto window = static_cast<GLFWwindow *>(hazel::Application::get_application()->get_window().get_native_window());
        int state = glfwGetKey(window, key_code);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::is_mouse_button_pressed_impl(unsigned int button) {
        auto window = static_cast<GLFWwindow *>(hazel::Application::get_application()->get_window().get_native_window());
        int state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::get_mouse_pos_impl() {
        auto window = static_cast<GLFWwindow *>(hazel::Application::get_application()->get_window().get_native_window());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return std::make_pair(xpos, ypos);
    }
} // namespace hazel::platform::linux