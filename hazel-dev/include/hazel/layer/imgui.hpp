#pragma once

#include <hazel/event/event.hpp>
#include <hazel/event/key.hpp>
#include <hazel/event/mouse.hpp>
#include <hazel/event/window.hpp>
#include <hazel/layer/layer.hpp>
#include <hazel/application.hpp>

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

namespace hazel::layer
{
    class ImGui : public Layer
    {
    public:
        ImGui();

        ~ImGui();

        void on_attach() override;

        void on_detach() override;

        void on_update() override;

        void on_event(hazel::event::Event &) override;

    private:
        float time = 0.0f;

        bool key_press_event(hazel::event::KeyPressEvent &);

        bool key_release_event(hazel::event::KeyReleaseEvent &);

        bool key_type_event(hazel::event::KeyTypeEvent &);

        bool on_mouse_button_press_event(hazel::event::MouseButtonPressEvent &);

        bool on_mouse_button_release_event(hazel::event::MouseButtonReleaseEvent &);

        bool mouse_move_event(hazel::event::MouseMoveEvent &);

        bool mouse_scroll_event(hazel::event::MouseScrollEvent &);

        bool window_resize_event(hazel::event::WindowResizeEvent &);
    };
} // namespace hazel::layer
