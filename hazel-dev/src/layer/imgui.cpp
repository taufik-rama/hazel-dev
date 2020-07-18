#include "hazel/layer/imgui.hpp"

#include <hazel/application.hpp>

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace hazel::layer
{
    ImGui::ImGui() : Layer("Layer::ImGui") {}

    void ImGui::on_attach()
    {
        // Setup Dear ImGui context
        ::IMGUI_CHECKVERSION();
        ::ImGui::CreateContext();
        ImGuiIO &io = ::ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ::ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle &style = ::ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer bindings
        auto window = static_cast<GLFWwindow *>(hazel::Application::get_application()->get_window().get_native_window());
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void ImGui::on_detach()
    {
        ::ImGui_ImplOpenGL3_Shutdown();
        ::ImGui_ImplGlfw_Shutdown();
        ::ImGui::DestroyContext();
    }

    // void ImGui::on_update()
    // {
    //     ::ImGuiIO &io = ::ImGui::GetIO();
    //     float time = glfwGetTime();
    //     io.DeltaTime = this->time > 0, 0 ? (time - this->time) : (1.0f / 60.0f);
    //     this->time = time;

    //     // Setup
    //     ::ImGui_ImplOpenGL3_NewFrame();
    //     ::ImGui::NewFrame();

    //     ::ImGui::ShowDemoWindow();

    //     // Render
    //     ::ImGui::Render();
    //     ::ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
    // }

    void ImGui::on_imgui_render()
    {
        static bool show = true;
        ::ImGui::ShowDemoWindow(&show);
    }

    void ImGui::begin()
    {
        ::ImGui_ImplOpenGL3_NewFrame();
        ::ImGui_ImplGlfw_NewFrame();
        ::ImGui::NewFrame();
    }

    void ImGui::end()
    {
        ::ImGuiIO &io = ::ImGui::GetIO();
        auto app = hazel::Application::get_application();
        io.DisplaySize = ::ImVec2(app->get_window().get_width(), app->get_window().get_height());

        ::ImGui::Render();
        ::ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());

        if (io.ConfigFlags & ::ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ::ImGui::UpdatePlatformWindows();
            ::ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    // void ImGui::on_event(hazel::event::Event &e)
    // {
    //     hazel::event::EventDispatcher dispatcher(e);
    //     dispatcher.dispatch<hazel::event::KeyPressEvent>(EVENT_BIND_METHOD_1(ImGui::key_press_event));
    //     dispatcher.dispatch<hazel::event::KeyReleaseEvent>(EVENT_BIND_METHOD_1(ImGui::key_release_event));
    //     dispatcher.dispatch<hazel::event::KeyTypeEvent>(EVENT_BIND_METHOD_1(ImGui::key_type_event));
    //     dispatcher.dispatch<hazel::event::MouseButtonPressEvent>(EVENT_BIND_METHOD_1(ImGui::on_mouse_button_press_event));
    //     dispatcher.dispatch<hazel::event::MouseButtonReleaseEvent>(EVENT_BIND_METHOD_1(ImGui::on_mouse_button_release_event));
    //     dispatcher.dispatch<hazel::event::MouseMoveEvent>(EVENT_BIND_METHOD_1(ImGui::mouse_move_event));
    //     dispatcher.dispatch<hazel::event::MouseScrollEvent>(EVENT_BIND_METHOD_1(ImGui::mouse_scroll_event));
    //     dispatcher.dispatch<hazel::event::WindowResizeEvent>(EVENT_BIND_METHOD_1(ImGui::window_resize_event));
    // }

    // bool ImGui::key_press_event(hazel::event::KeyPressEvent &e)
    // {
    //     ::ImGuiIO &io = ::ImGui::GetIO();
    //     io.KeysDown[e.get_key_code()] = true;
    //     io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    //     io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    //     io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    //     io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
    //     return false;
    // }

    // bool ImGui::key_release_event(hazel::event::KeyReleaseEvent &e)
    // {
    //     ::ImGuiIO &io = ::ImGui::GetIO();
    //     io.KeysDown[e.get_key_code()] = false;
    //      io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    //     io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    //     io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    //     io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
    //     return false;
    // }

    // bool ImGui::key_type_event(hazel::event::KeyTypeEvent &e)
    // {
    //     ::ImGuiIO &io = ::ImGui::GetIO();
    //     io.AddInputCharacter(e.get_key_code());
    //     return false;
    // }

    // bool ImGui::on_mouse_button_press_event(hazel::event::MouseButtonPressEvent &e)
    // {
    //     ::ImGuiIO &io = ::ImGui::GetIO();
    //     io.MouseDown[e.get_mouse_code()] = true;
    //     return false;
    // }

    // bool ImGui::on_mouse_button_release_event(hazel::event::MouseButtonReleaseEvent &e)
    // {
    //     ::ImGuiIO &io = ::ImGui::GetIO();
    //     io.MouseDown[e.get_mouse_code()] = false;
    //     return false;
    // }

    // bool ImGui::mouse_move_event(hazel::event::MouseMoveEvent &e)
    // {
    //     ::ImGuiIO &io = ::ImGui::GetIO();
    //     io.MousePos = ::ImVec2(e.get_x(), e.get_y());
    //     return false;
    // }

    // bool ImGui::mouse_scroll_event(hazel::event::MouseScrollEvent &e)
    // {
    //     ::ImGuiIO &io = ::ImGui::GetIO();
    //     io.MouseWheel += e.get_y_offset();
    //     io.MouseWheelH += e.get_x_offset();
    //     return false;
    // }

    // bool ImGui::window_resize_event(hazel::event::WindowResizeEvent &e)
    // {
    //     ::ImGuiIO &io = ::ImGui::GetIO();
    //     io.DisplaySize = ImVec2(e.get_width(), e.get_height());
    //     // glViewport(0, 0, e.get_width(), e.get_height());
    //     return false;
    // }

} // namespace hazel::layer