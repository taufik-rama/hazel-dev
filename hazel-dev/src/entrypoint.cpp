#include <entrypoint.hpp>

int main(/* int arc, char **argv */)
{
    hazel::logger::init();
    HAZEL_DEV_LOG_INFO("{}, {}", HAZEL_NAME, HAZEL_VERSION);
    HAZEL_CLIENT_LOG_INFO("{}, {}", HAZEL_NAME, HAZEL_VERSION);

    auto app = hazel::create_application();
    app->Run();
    delete app;
}