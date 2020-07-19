#pragma once

#include <hazel/renderer/library.hpp>

namespace hazel::platform::linux
{
    class Library : public hazel::renderer::Library
    {
        virtual void set_clear_color(const glm::vec4 &color) override;

        virtual void clear_color() override;

        virtual void draw_index(const hazel::Ref<hazel::renderer::VertexArray> &va) override;
    };
} // namespace hazel::platform::linux