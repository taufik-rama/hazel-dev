#include <hazel/core/entrypoint.hpp>

#include <hazel/core/application.hpp>
#include <hazel/meta.hpp>

int main(/* int arc, char **argv */) {
  hazel::core::Logger::init();
  HAZEL_DEV_LOG_INFO("{}, {}", HAZEL_NAME, HAZEL_VERSION);

  hazel::trace::Profiler::get()->begin_session("setup-profile",
                                               "setup-profile.json");
  auto app = hazel::core::create_application();
  hazel::trace::Profiler::get()->end_session();

  hazel::trace::Profiler::get()->begin_session("run-profile",
                                               "run-profile.json");
  app->run();
  hazel::trace::Profiler::get()->end_session();

  hazel::trace::Profiler::get()->begin_session("final-profile",
                                               "final-profile.json");
  app.reset();
  hazel::trace::Profiler::get()->end_session();
}
