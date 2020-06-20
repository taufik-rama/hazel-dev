#include "hazel/layer/imgui.hpp"

// Temporary, glClearColor
#include <glad/glad.h>

namespace hazel::layer
{
    ImGui::ImGui() : Layer("Layer::ImGui") {}

    ImGui::~ImGui() {}

    void ImGui::on_attach()
    {
        ::ImGui::CreateContext();
        ::ImGui::StyleColorsDark();
        ::ImGuiIO &io = ::ImGui::GetIO();
        io.BackendFlags |= ::ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ::ImGuiBackendFlags_HasSetMousePos;
        io.KeyMap[::ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[::ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[::ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[::ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[::ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[::ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[::ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[::ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[::ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[::ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[::ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[::ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[::ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[::ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[::ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[::ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[::ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[::ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[::ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[::ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[::ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[::ImGuiKey_Z] = GLFW_KEY_Z;

        auto app = hazel::Application::get_application();
        io.DisplaySize = ::ImVec2(app->get_window().get_width(), app->get_window().get_height());

        ::ImGui_ImplOpenGL3_Init("#version 460");
    }

    void ImGui::on_detach() {}

    void ImGui::on_update()
    {
        ::ImGuiIO &io = ::ImGui::GetIO();
        float time = glfwGetTime();
        io.DeltaTime = this->time > 0, 0 ? (time - this->time) : (1.0f / 60.0f);
        this->time = time;

        // Setup
        ::ImGui_ImplOpenGL3_NewFrame();
        ::ImGui::NewFrame();

        ::ImGui::ShowDemoWindow();

        // Render
        ::ImGui::Render();
        ::ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
    }

    void ImGui::on_event(hazel::event::Event &e)
    {
        hazel::event::EventDispatcher dispatcher(e);
        dispatcher.dispatch<hazel::event::KeyPressEvent>(EVENT_BIND_METHOD_1(ImGui::key_press_event));
        dispatcher.dispatch<hazel::event::KeyReleaseEvent>(EVENT_BIND_METHOD_1(ImGui::key_release_event));
        dispatcher.dispatch<hazel::event::KeyTypeEvent>(EVENT_BIND_METHOD_1(ImGui::key_type_event));
        dispatcher.dispatch<hazel::event::MouseButtonPressEvent>(EVENT_BIND_METHOD_1(ImGui::on_mouse_button_press_event));
        dispatcher.dispatch<hazel::event::MouseButtonReleaseEvent>(EVENT_BIND_METHOD_1(ImGui::on_mouse_button_release_event));
        dispatcher.dispatch<hazel::event::MouseMoveEvent>(EVENT_BIND_METHOD_1(ImGui::mouse_move_event));
        dispatcher.dispatch<hazel::event::MouseScrollEvent>(EVENT_BIND_METHOD_1(ImGui::mouse_scroll_event));
        dispatcher.dispatch<hazel::event::WindowResizeEvent>(EVENT_BIND_METHOD_1(ImGui::window_resize_event));
    }

    bool ImGui::key_press_event(hazel::event::KeyPressEvent &e)
    {
        ::ImGuiIO &io = ::ImGui::GetIO();
        io.KeysDown[e.get_key_code()] = true;
        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
        return false;
    }

    bool ImGui::key_release_event(hazel::event::KeyReleaseEvent &e)
    {
        ::ImGuiIO &io = ::ImGui::GetIO();
        io.KeysDown[e.get_key_code()] = false;
         io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
        return false;
    }

    bool ImGui::key_type_event(hazel::event::KeyTypeEvent &e)
    {
        ::ImGuiIO &io = ::ImGui::GetIO();
        io.AddInputCharacter(e.get_key_code());
        return false;
    }

    bool ImGui::on_mouse_button_press_event(hazel::event::MouseButtonPressEvent &e)
    {
        ::ImGuiIO &io = ::ImGui::GetIO();
        io.MouseDown[e.get_mouse_code()] = true;
        return false;
    }

    bool ImGui::on_mouse_button_release_event(hazel::event::MouseButtonReleaseEvent &e)
    {
        ::ImGuiIO &io = ::ImGui::GetIO();
        io.MouseDown[e.get_mouse_code()] = false;
        return false;
    }

    bool ImGui::mouse_move_event(hazel::event::MouseMoveEvent &e)
    {
        ::ImGuiIO &io = ::ImGui::GetIO();
        io.MousePos = ::ImVec2(e.get_x(), e.get_y());
        return false;
    }

    bool ImGui::mouse_scroll_event(hazel::event::MouseScrollEvent &e)
    {
        ::ImGuiIO &io = ::ImGui::GetIO();
        io.MouseWheel += e.get_y_offset();
        io.MouseWheelH += e.get_x_offset();
        return false;
    }

    bool ImGui::window_resize_event(hazel::event::WindowResizeEvent &e)
    {
        ::ImGuiIO &io = ::ImGui::GetIO();
        io.DisplaySize = ImVec2(e.get_width(), e.get_height());
        // glViewport(0, 0, e.get_width(), e.get_height());
        return false;
    }

} // namespace hazel::layer