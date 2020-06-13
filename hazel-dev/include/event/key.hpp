#pragma once

#include <sstream>

#include <event/event.hpp>

namespace hazel::event
{
    class KeyEvent : public Event
    {
    public:
        int get_key_code() const { return this->key_code; }

        EVENT_TYPE_CATEGORY_METHODS(
            EventTypeCategorySet2(
                EventTypeCategory::INPUT,
                EventTypeCategory::KEY))

    protected:
        int key_code;

        KeyEvent(int key_code) : key_code(key_code) {}
    };

    class KeyPressEvent : public KeyEvent
    {
    public:
        KeyPressEvent(int key_code, int repeat_count) : KeyEvent(key_code), repeat_count(repeat_count) {}

        int get_repeat_count() const { return this->repeat_count; }

        std::string to_string() const override
        {
            std::stringstream ss;
            ss << "KeyPressEvent: " << this->key_code << "(" << this->repeat_count << " repeats)";
            return ss.str();
        }

        EVENT_TYPE_METHODS(KEY_PRESS)

    private:
        int repeat_count;
    };

    class KeyReleaseEvent : public KeyEvent
    {
    public:
        std::string to_string() const override
        {
            std::stringstream ss;
            ss << "KeyPressEvent: " << this->key_code;
            return ss.str();
        }

        EVENT_TYPE_METHODS(KEY_RELEASE)
    };
} // namespace hazel::event
