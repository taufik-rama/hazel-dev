#pragma once

namespace hazel::renderer
{
    enum class RendererAPI
    {
        NONE = 0,
        OPENGL = 1,
    };

    class Renderer
    {
    public:
        virtual ~Renderer() {}

        static RendererAPI get_api()
        {
            return renderer_api;
        }

    private:
        static RendererAPI renderer_api;
    };
} // namespace hazel::renderer