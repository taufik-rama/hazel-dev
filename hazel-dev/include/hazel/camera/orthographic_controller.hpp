#pragma once

#include <hazel/camera/orthographic.hpp>
#include <hazel/core/timestep.hpp>
#include <hazel/event/mouse.hpp>
#include <hazel/event/window.hpp>
#include <hazel/core.hpp>

namespace hazel::camera
{
    class OrthographicController
    {
    public:
        OrthographicController(float aspect_ratio, bool rotation = false);

        Orthographic &get_camera();

        void on_update(hazel::core::Timestep);

        void on_event(hazel::event::Event &);

    private:
        bool on_mouse_scroll(hazel::event::MouseScrollEvent &);

        bool on_window_resize(hazel::event::WindowResizeEvent &);

        float aspect_ratio, zoom_level = 1.0f;

        Orthographic camera;

        bool rotation;
        glm::vec3 camera_pos = {0.0f, 0.0f, 0.0f};
        float camera_pos_speed = 5.0f, camera_rot = 0.0f, camera_rot_speed = 180.0f;
    };
} // namespace hazel::camera
