#pragma once

#include <event/event.hpp>

namespace hazel::event
{
    class AppTickEvent : public Event
    {
    };

    class AppUpdateEvent : public Event
    {
    };

    class AppRenderEvent : public Event
    {
    };
} // namespace hazel::event
