#include <layer.hpp>

AppLayer::AppLayer()
    : hazel::layer::Layer("Application Layer") // 16:9 aspect ratio
{
  this->init_camera();

  auto io = ImGui::GetIO();
  this->font = io.Fonts->AddFontFromFileTTF(
      "../assets/fonts/OpenSans-Regular.ttf", 120.0f);
}

AppLayer::~AppLayer() {}

void AppLayer::on_attach() { this->level.init(); }

void AppLayer::on_detach() {}

void AppLayer::on_update(hazel::core::Timestep ts) {

  if (this->level.is_game_over()) {
    this->game_state = GameState::GAME_OVER;
    // return;
  }

  // On each update, we can set the "background color".
  // It uses buffer, so the actual color will be changed after `clear_color`
  hazel::renderer::Command::set_clear_color({0.2, 0.2, 0.2, 1});
  hazel::renderer::Command::clear_color();

  // Set the camera to always follow the player
  auto player_pos = this->level.get_player_ref().get_position();
  this->camera->set_position({player_pos.x, player_pos.y, 0.0f});

  switch (this->game_state) {
  case GameState::PLAY: {
    this->level.on_update(ts);
    break;
  }
  default: {
  }
  }

  hazel::renderer::Renderer2D::begin_scene(*this->camera);
  this->level.on_render();
  hazel::renderer::Renderer2D::end_scene();
}

void AppLayer::on_event(hazel::event::Event &e) {
  hazel::event::EventDispatcher dispatcher(e);
  dispatcher.dispatch<hazel::event::MouseButtonReleaseEvent>(
      EVENT_BIND_METHOD_1(AppLayer::on_mouse_button_release));
}

void AppLayer::on_imgui_render() {
  switch (this->game_state) {
  case GameState::PLAY: {
    break;
  }
  case GameState::MAIN_MENU: {
    auto pos = ImGui::GetWindowPos();
    auto window_width =
        hazel::core::Application::get_application()->get_window().get_width();
    pos.x += window_width * 0.5 - 300.0f;
    auto window_height =
        hazel::core::Application::get_application()->get_window().get_height();
    pos.y += window_height * 0.5f + 50.0f;
    unsigned int rgba = 0xFFFFFFFF;
    ImGui::GetForegroundDrawList()->AddText(this->font, 120.0f, pos, rgba,
                                            "Click to play!");
    break;
  }
  case GameState::GAME_OVER: {
    auto pos = ImGui::GetWindowPos();
    auto window_width =
        hazel::core::Application::get_application()->get_window().get_width();
    pos.x += window_width * 0.5 - 300.0f;
    auto window_height =
        hazel::core::Application::get_application()->get_window().get_height();
    pos.y += window_height * 0.5f + 50.0f;
    unsigned int rgba = 0xFFFFFFFF;
    ImGui::GetForegroundDrawList()->AddText(this->font, 120.0f, pos, rgba,
                                            "ded (click again)");
    break;
  }
  }
}

void AppLayer::init_camera() {
  auto &window = hazel::core::Application::get_application()->get_window();
  float aspect_ratio = (float)window.get_width() / (float)window.get_height();

  float zoom_level = 24.0f;
  this->camera = hazel::core::create_scope<hazel::camera::Orthographic>(
      (-aspect_ratio * zoom_level), (aspect_ratio * zoom_level), -zoom_level,
      zoom_level);
}

bool AppLayer::on_mouse_button_release(
    hazel::event::MouseButtonReleaseEvent &) {
  // Don't reset if it's already playing
  switch (this->game_state) {
  case GameState::PLAY:
    break;
  default:
    this->game_state = GameState::PLAY;
    this->level.reset();
    break;
  }
  return false;
}
