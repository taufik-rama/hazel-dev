#include <app.hpp>

App::App() {
  hazel::core::Application::get_application()->get_window().set_title(
      "FLAPPY SHIPP!!!1");
  this->main_layer = hazel::core::create_ref<AppLayer>();
  this->add_layer(this->main_layer);
}

App::~App() {}

hazel::core::Scope<hazel::core::Application> hazel::core::create_application() {
  return hazel::core::create_scope<App>();
}
