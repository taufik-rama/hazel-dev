#pragma once

#include <hazel/renderer/library.hpp>

namespace hazel::platform::linux
{
    class Library : public hazel::renderer::Library
    {
        virtual void init() override;

        virtual void set_clear_color(const glm::vec4 &color) override;

        virtual void clear_color() override;

        virtual void draw_index(const hazel::Ref<hazel::renderer::VertexArray> &va) override;

        virtual void set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
    };
} // namespace hazel::platform::linux