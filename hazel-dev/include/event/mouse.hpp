#pragma once

#include <sstream>

#include <event/event.hpp>

namespace hazel::event
{
    class MousePressEvent : public Event
    {
    public:
        int get_mouse_button() const { return 0; }
    };

    class MouseReleaseEvent : public Event
    {
    public:
        int get_mouse_button() const { return 0; }
    };

    class MouseMoveEvent : public Event
    {
    public:
        float get_x() const { return 0; }

        float get_y() const { return 0; }

        std::string to_string() const override
        {
            std::stringstream ss;
            ss << "MouseMoveEvent: ";
            return ss.str();
        }
    };

    class MouseScrollEvent : public Event
    {
    public:
        float get_x_offset() const { return 0; }

        float get_y_offset() const { return 0; }

        std::string to_string() const override
        {
            std::stringstream ss;
            ss << "MouseScrollEvent: ";
            return ss.str();
        }
    };
} // namespace hazel::event
