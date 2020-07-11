#pragma once

#include <hazel/layer/layer.hpp>

namespace hazel::layer
{
    class ImGui : public Layer
    {
    public:
        ImGui();

        ~ImGui();

        virtual void on_attach() override;

        virtual void on_detach() override;

        // virtual void on_update() override;

        virtual void on_imgui_render() override;

        void begin();

        void end();

        // void on_event(hazel::event::Event &) override;

    private:
        float time = 0.0f;

        // bool key_press_event(hazel::event::KeyPressEvent &);

        // bool key_release_event(hazel::event::KeyReleaseEvent &);

        // bool key_type_event(hazel::event::KeyTypeEvent &);

        // bool on_mouse_button_press_event(hazel::event::MouseButtonPressEvent &);

        // bool on_mouse_button_release_event(hazel::event::MouseButtonReleaseEvent &);

        // bool mouse_move_event(hazel::event::MouseMoveEvent &);

        // bool mouse_scroll_event(hazel::event::MouseScrollEvent &);

        // bool window_resize_event(hazel::event::WindowResizeEvent &);
    };
} // namespace hazel::layer
