#pragma once

#include <hazel/renderer/command.hpp>
#include <hazel/renderer/vertex_array.hpp>

namespace hazel::renderer
{
    class Renderer
    {
    public:
        static Library::API get_api_library()
        {
            return Library::get_api_library();
        }

        static void begin_scene();

        static void end_scene();

        static void submit(const std::shared_ptr<VertexArray> &va);
    };
} // namespace hazel::renderer