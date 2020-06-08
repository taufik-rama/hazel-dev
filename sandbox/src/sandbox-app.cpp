#include <iostream>

#include <hazel-api.hpp>

class Sandbox : public hazel::Application
{
public:
    Sandbox()
    {
    }

    ~Sandbox()
    {
    }
};

hazel::Application *hazel::create_application()
{
    return new Sandbox();
}
