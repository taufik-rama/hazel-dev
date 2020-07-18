#pragma once

#include <hazel/core.hpp>

namespace hazel::renderer
{
    class GraphicsContext
    {
    public:
        virtual void init() = 0;

        virtual void swap_buffers() = 0;
    };
} // namespace hazel::renderer
