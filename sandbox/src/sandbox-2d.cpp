#include <sandbox-2d.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

static float rotation = 0.0f;

Sandbox2D::Sandbox2D()
    : hazel::layer::Layer("Sandbox2D Example Layer"),
      camera_controller((float)16 / (float)9, true) {} // 16:9 aspect ratio

Sandbox2D::~Sandbox2D() {}

void Sandbox2D::on_attach() {
  this->checkerboard_texture =
      hazel::renderer::Texture2D::create("../assets/textures/Checkerboard.png");
}

void Sandbox2D::on_detach() {}

void Sandbox2D::on_update(hazel::core::Timestep ts) {
  TIMER_SCOPE();
  {
    TIMER_SCOPE_NAME("camera::update");
    this->camera_controller.on_update(ts);
  }

  {
    TIMER_SCOPE_NAME("renderer::setup");
    hazel::renderer::Command::set_clear_color({0.2, 0.2, 0.2, 1});
    hazel::renderer::Command::clear_color();
  }

  {
    TIMER_SCOPE_NAME("renderer::draw");
    hazel::renderer::Renderer2D::begin_scene(
        this->camera_controller.get_camera());
    hazel::renderer::Renderer2D::draw_quad(
        {-0.6f, 0.4f}, rotation, {0.4f, 0.4f}, {0.8f, 0.2f, 0.3f, 1.0f});
    hazel::renderer::Renderer2D::draw_quad({0.0f, 0.0f, -0.1f}, rotation,
                                           {10.0f, 10.0f},
                                           this->checkerboard_texture);
    hazel::renderer::Renderer2D::end_scene();
  }
}

void Sandbox2D::on_event(hazel::event::Event &e) {
  this->camera_controller.on_event(e);
}

void Sandbox2D::on_imgui_render() {
  TIMER_SCOPE();
  ImGui::Begin("Settings");
  ImGui::ColorEdit4("Square Array Color", glm::value_ptr(this->square_array));
  ImGui::DragFloat("Rotation", &rotation);
  ImGui::End();
}
