#include <hazel/core/entrypoint.hpp>

#include <hazel/core/application.hpp>
#include <hazel/meta.hpp>

int main(/* int arc, char **argv */) {
  hazel::core::Logger::init();
  HAZEL_DEV_LOG_INFO("{}, {}", HAZEL_NAME, HAZEL_VERSION);

  auto app = hazel::core::create_application();
  app->run();
}
