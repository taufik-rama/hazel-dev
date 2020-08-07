#pragma once

#include <hazel/renderer/library.hpp>
#include <hazel/renderer/vertex_array.hpp>

#include <glm/glm.hpp>

namespace hazel::renderer
{
    class Command
    {
    public:
        static void init();

        static void set_clear_color(const glm::vec4 &color);

        static void set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

        static void clear_color();

        static void draw_index(const hazel::Ref<VertexArray> &va);

    private:
        static hazel::Ref<Library> library;
    };
} // namespace hazel::renderer