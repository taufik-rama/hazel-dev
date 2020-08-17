#include "widgets.hpp"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>

namespace ImGuiWidgets {
IMGUI_API void RenderContainer(ImVec2 size) {
  ImGuiWindow *window = ImGui::GetCurrentWindow();
  const ImRect frame(window->DC.CursorPos, window->DC.CursorPos + size);
  ImGui::RenderFrame(frame.Min, frame.Max, ImGui::GetColorU32(ImGuiCol_FrameBg),
                     true);
}
} // namespace ImGuiWidgets