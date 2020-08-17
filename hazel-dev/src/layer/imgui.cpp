#include <hazel/layer/imgui.hpp>

#include <hazel/core/application.hpp>

#include <GLFW/glfw3.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <imgui.h>

namespace hazel::layer {
ImGui::ImGui() : Layer("Layer::ImGui") {}

void ImGui::on_attach() {
  // Setup Dear ImGui context
  ::IMGUI_CHECKVERSION();
  ::ImGui::CreateContext();
  ImGuiIO &io = ::ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad
  // Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /
                                                      // Platform Windows
  // io.ConfigViewportsNoAutoMerge = true;
  // io.ConfigViewportsNoTaskBarIcon = true;

  // Setup Dear ImGui style
  ::ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle &style = ::ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer bindings
  auto window =
      static_cast<GLFWwindow *>(hazel::core::Application::get_application()
                                    ->get_window()
                                    .get_native_window());
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGui::on_detach() {
  ::ImGui_ImplOpenGL3_Shutdown();
  ::ImGui_ImplGlfw_Shutdown();
  ::ImGui::DestroyContext();
}

void ImGui::begin() {
  ::ImGui_ImplOpenGL3_NewFrame();
  ::ImGui_ImplGlfw_NewFrame();
  ::ImGui::NewFrame();
}

void ImGui::end() {
  ::ImGuiIO &io = ::ImGui::GetIO();
  auto app = hazel::core::Application::get_application();
  io.DisplaySize =
      ::ImVec2(app->get_window().get_width(), app->get_window().get_height());

  ::ImGui::Render();
  ::ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());

  if (io.ConfigFlags & ::ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ::ImGui::UpdatePlatformWindows();
    ::ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

} // namespace hazel::layer