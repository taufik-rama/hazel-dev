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

namespace hazel::event
{
    enum class EventType
    {
        NONE = 0,
        WINDOW_CLOSE,
        WINDOW_RESIZE,
        WINDOW_FOCUS,
        WINDOW_LOST_FOCUS,
        WINDOW_MOVE,
        APP_TICK,
        APP_UPDATE,
        APP_RENDER,
        KEY_PRESS,
        KEY_RELEASE,
        MOUSE_PRESS,
        MOUSE_RELEASE,
        MOUSE_MOVE,
        MOUSE_SCROLL
    };

    enum class EventTypeCategory
    {
        NONE = 0,
        WINDOW = BIT(0),
        APP = BIT(1),
        INPUT = BIT(2),
        KEY = BIT(3),
        MOUSE = BIT(4)
    };

    class Event
    {
        friend class EventDispatcher;

    public:
        virtual EventType get_event_type() const = 0;

        virtual unsigned int get_event_type_categories() const = 0;

        virtual const char *get_name() const = 0;

        virtual std::string to_string() const { return this->get_name(); }

        bool is_in_category(EventTypeCategory category) const
        {
            return (static_cast<unsigned int>(category) & get_event_type_categories());
        }

        template<typename OStream>
        friend OStream &operator<<(OStream &os, const Event &e) {
            return os << e.to_string();
        }

    protected:
        bool handled = false;
    };

    class EventDispatcher
    {
        template <typename T>
        using event_fn = std::function<bool(T &)>;

    public:
        EventDispatcher(Event &event) : event(event) {}

        template <typename T>
        bool dispatch(event_fn<T> fn)
        {
            if (this->event.get_event_type() == T::GetStaticType())
            {
                this->event.handled = fn(*(T *)(&this->event));
                // this->event.handled = fn(7);
                return true;
            }
            return false;
        }

    private:
        Event &event;
    };

    // std::ostream &operator<<(std::ostream &os, Event &e)
    // {
    //     return os << e.to_string();
    // }
} // namespace hazel::event
