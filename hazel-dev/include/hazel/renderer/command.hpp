#pragma once

#include <hazel/renderer/library.hpp>
#include <hazel/renderer/vertex_array.hpp>

#include <glm/glm.hpp>

namespace hazel::renderer
{
    class Command
    {
    public:
        static void set_clear_color(const glm::vec4 &color);

        static void clear_color();

        static void draw_index(const std::shared_ptr<VertexArray> &va);

    private:
        static Library* api_library;
    };
} // namespace hazel::renderer