#include <iostream>

#include <hazel-api.hpp>

class ExampleLayer : public hazel::layer::Layer
{
public:
    ExampleLayer() : hazel::layer::Layer("Sandbox Example Layer") {}

    void on_update() override {}

    void on_event(hazel::event::Event &e) override
    {
        if (hazel::Input::is_key_pressed(HAZEL_KEY_TAB))
        {
            HAZEL_CLIENT_LOG_TRACE("Tab key is pressed", "");
        }
        else
        {
            HAZEL_CLIENT_LOG_TRACE("ExampleLayer on_event: {}", e);
        }
    }
};

class Sandbox : public hazel::Application
{
public:
    Sandbox()
    {
        this->add_layer(new ExampleLayer());
        this->add_layer_overlay(new hazel::layer::ImGui());
    }

    ~Sandbox() {}
};

hazel::Application *hazel::create_application()
{
    return new Sandbox();
}
