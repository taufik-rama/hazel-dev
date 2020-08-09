#pragma once

#include <hazel/event/event.hpp>

namespace hazel::event {
class MouseButtonEvent : public Event {
public:
  int get_mouse_code() const { return this->mouse_code; }

  EVENT_TYPE_CATEGORY_METHODS(EventTypeCategorySet1(EventTypeCategory::MOUSE))

protected:
  int mouse_code;

  MouseButtonEvent(int mouse_code) : mouse_code(mouse_code) {}
};

class MouseButtonPressEvent : public MouseButtonEvent {
public:
  MouseButtonPressEvent(int mouse_code) : MouseButtonEvent(mouse_code) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "MouseButtonPressEvent: mouse_code: " << this->mouse_code;
    return ss.str();
  }

  EVENT_TYPE_METHODS(MOUSE_PRESS)
};

class MouseButtonReleaseEvent : public MouseButtonEvent {
public:
  MouseButtonReleaseEvent(int mouse_code) : MouseButtonEvent(mouse_code) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "MouseButtonReleaseEvent: mouse_code: " << this->mouse_code;
    return ss.str();
  }

  EVENT_TYPE_METHODS(MOUSE_RELEASE)
};

class MouseMoveEvent : public Event {
public:
  MouseMoveEvent(float x, float y) : x(x), y(y) {}

  float get_x() const { return this->x; }

  float get_y() const { return this->y; }

  std::string to_string() const override {
    std::stringstream ss;
    ss << "MouseMoveEvent: x: " << this->x << ", y: " << this->y;
    return ss.str();
  }

  EVENT_TYPE_CATEGORY_METHODS(EventTypeCategorySet1(EventTypeCategory::MOUSE))

  EVENT_TYPE_METHODS(MOUSE_MOVE)

private:
  float x, y;
};

class MouseScrollEvent : public Event {
public:
  MouseScrollEvent(float x_offset, float y_offset)
      : x_offset(x_offset), y_offset(y_offset) {}

  float get_x_offset() const { return this->x_offset; }

  float get_y_offset() const { return this->y_offset; }

  std::string to_string() const override {
    std::stringstream ss;
    ss << "MouseScrollEvent: x_offset: " << this->x_offset
       << ", y_offset: " << this->y_offset;
    return ss.str();
  }

  EVENT_TYPE_CATEGORY_METHODS(EventTypeCategorySet1(EventTypeCategory::MOUSE))

  EVENT_TYPE_METHODS(MOUSE_SCROLL)

private:
  float x_offset, y_offset;
};
} // namespace hazel::event
