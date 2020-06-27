#pragma once

#include <hazel/event/event.hpp>
#include <hazel/core.hpp>

namespace hazel::layer
{
    class Layer
    {
    public:
        Layer(const std::string &name = "Layer") : debug_name(name) {}

        virtual ~Layer() {}

        virtual void on_attach() {}

        virtual void on_detach() {}

        virtual void on_update() {}

        virtual void on_event(hazel::event::Event &) {}

        virtual void on_imgui_render() {}

        const std::string &to_string() const { return this->debug_name; }

    private:
        std::string debug_name;
    };
} // namespace hazel::layer
