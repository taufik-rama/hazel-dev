#pragma once

#include <hazel/event/event.hpp>

namespace hazel::event
{
    class WindowCloseEvent : public Event
    {
    };

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height) : width(width), height(height) {}

        unsigned int get_width() const { return this->width; }

        unsigned int get_height() const { return this->height; }

        EVENT_TYPE_CATEGORY_METHODS(EventTypeCategorySet1(EventTypeCategory::WINDOW))
        EVENT_TYPE_METHODS(WINDOW_RESIZE)

    private:
        int width, height;
    };

    class WindowFocusEvent : public Event
    {
    };

    class WindowLostFocusEvent : public Event
    {
    };

    class WindowMoveEvent : public Event
    {
    };
} // namespace hazel::event