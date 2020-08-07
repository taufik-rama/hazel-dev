#pragma once

#include <hazel/core.hpp>

#define EVENT_TYPE_METHODS(type)                                                                \
    static EventType get_static_event_type() { return EventType::type; }                        \
    virtual EventType get_event_type() const override { return this->get_static_event_type(); } \
    virtual const char *get_name() const override { return #type; }

#define EVENT_TYPE_CATEGORY_METHODS(category) \
    virtual unsigned int get_event_type_categories() const override { return category; }

#define EventTypeCategorySet1(a) static_cast<unsigned int>(a)
#define EventTypeCategorySet2(a, b) static_cast<unsigned int>(a) | static_cast<unsigned int>(b)
#define EventTypeCategorySet3(a, b, c) static_cast<unsigned int>(a) | static_cast<unsigned int>(b) | static_cast<unsigned int>(c)

#define EVENT_BIND_METHOD_1(x) std::bind(&x, this, std::placeholders::_1)

namespace hazel::event
{
    enum class EventType
    {
        NONE = 0,
        WINDOW_CLOSE,
        WINDOW_RESIZE,
        WINDOW_MINIMIZE,
        WINDOW_FOCUS,
        WINDOW_LOST_FOCUS,
        WINDOW_MOVE,
        APP_TICK,
        APP_UPDATE,
        APP_RENDER,
        KEY_PRESS,
        KEY_RELEASE,
        KEY_TYPE,
        MOUSE_PRESS,
        MOUSE_RELEASE,
        MOUSE_MOVE,
        MOUSE_SCROLL
    };

    enum class EventTypeCategory
    {
        NONE = 0,
        WINDOW = SHIFT(0),
        APP = SHIFT(1),
        INPUT = SHIFT(2),
        KEY = SHIFT(3),
        MOUSE = SHIFT(4)
    };

    class Event
    {
        friend class EventDispatcher;

    public:
        virtual EventType get_event_type() const = 0;

        virtual unsigned int get_event_type_categories() const = 0;

        virtual const char *get_name() const = 0;

        bool is_in_category(EventTypeCategory category) const
        {
            return (static_cast<unsigned int>(category) & get_event_type_categories());
        }

        bool is_handled() const { return this->handled; }

        virtual std::string to_string() const { return this->get_name(); }
        FORMAT_TO_STRING(Event)

    protected:
        bool handled = false;
    };

    class EventDispatcher
    {
        template <typename T>
        using EventFn = std::function<bool(T &)>;

    public:
        EventDispatcher(Event &e) : event(e) {}

        template <typename T>
        bool dispatch(EventFn<T> fn)
        {
            if (this->event.get_event_type() == T::get_static_event_type())
            {
                this->event.handled = fn(*(T *)(&this->event));
                return true;
            }
            return false;
        }

    private:
        Event &event;
    };
} // namespace hazel::event
