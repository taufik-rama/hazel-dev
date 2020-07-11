#include <hazel/entrypoint.hpp>

#include <hazel/application.hpp>
#include <hazel/meta.hpp>

int main(/* int arc, char **argv */)
{
    hazel::Logger::init();
    HAZEL_DEV_LOG_INFO("{}, {}", HAZEL_NAME, HAZEL_VERSION);

    auto app = hazel::create_application();
    app->run();
    delete app;
}
