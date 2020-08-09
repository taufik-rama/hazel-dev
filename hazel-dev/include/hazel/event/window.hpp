#pragma once

#include <hazel/event/event.hpp>

namespace hazel::event {
class WindowCloseEvent : public Event {
public:
  EVENT_TYPE_CATEGORY_METHODS(EventTypeCategorySet1(EventTypeCategory::WINDOW))
  EVENT_TYPE_METHODS(WINDOW_CLOSE)
};

class WindowResizeEvent : public Event {
public:
  WindowResizeEvent(unsigned int width, unsigned int height)
      : width(width), height(height) {}

  unsigned int get_width() const { return this->width; }

  unsigned int get_height() const { return this->height; }

  std::string to_string() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent: width: " << this->width
       << ", height: " << this->height;
    return ss.str();
  }

  EVENT_TYPE_CATEGORY_METHODS(EventTypeCategorySet1(EventTypeCategory::WINDOW))
  EVENT_TYPE_METHODS(WINDOW_RESIZE)

private:
  int width, height;
};

class WindowMinimizeEvent : public Event {
public:
  WindowMinimizeEvent(bool minimized) : minimized(minimized) {}

  bool is_minimized() const { return this->minimized; }

  std::string to_string() const override {
    std::stringstream ss;
    ss << "WindowMinimizeEvent: minimized: " << this->minimized;
    return ss.str();
  }

  EVENT_TYPE_CATEGORY_METHODS(EventTypeCategorySet1(EventTypeCategory::WINDOW))
  EVENT_TYPE_METHODS(WINDOW_MINIMIZE)

private:
  bool minimized;
};

class WindowFocusEvent : public Event {};

class WindowLostFocusEvent : public Event {};

class WindowMoveEvent : public Event {};
} // namespace hazel::event