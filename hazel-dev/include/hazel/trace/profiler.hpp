#pragma once

#include <hazel/core/memory.hpp>

namespace hazel::trace {
struct profiler_data {
  const char *name;
  long long start, end;
  std::thread::id thread_id;
};

class Profiler {

public:
  static hazel::core::Ref<Profiler> get() {
    if (profiler) {
      return profiler;
    }
    profiler = hazel::core::create_ref<Profiler>();
    return profiler;
  }

private:
  inline static hazel::core::Ref<Profiler> profiler;

public:
  Profiler() : profiler_count(0) {}

  void begin_session(const char *name,
                     const std::string &filepath = "profiler-results.json") {
    this->output_stream.open(filepath);
    this->write_header(name);
  }

  void end_session() {
    this->write_footer();
    this->profiler_count = 0;
    this->output_stream.close();
  }

  void write_session(const profiler_data &data) {
    if (this->profiler_count > 0) {
      this->output_stream << ",";
    }
    this->profiler_count++;

    this->output_stream << R"({"cat": "function", "ph": "X", "pid": 0, )";
    this->output_stream << "\"dur\": " << (data.end - data.start) << ", ";
    this->output_stream << "\"name\":" << '\"' << data.name << '\"' << ", ";
    this->output_stream << "\"tid\":" << data.thread_id << ", ";
    this->output_stream << "\"ts\":" << data.start;
    this->output_stream << "}";
    this->output_stream.flush();
  }

  void write_header(const char *name) {
    this->output_stream << R"({"otherData": {)"
                        << "\"name\": \"" << name << "\""
                        << R"(}, "traceEvents": [)";
    this->output_stream.flush();
  }

  void write_footer() {
    this->output_stream << R"(]})";
    this->output_stream.flush();
  }

private:
  unsigned int profiler_count;
  std::ofstream output_stream;
};

#define TIMER_SCOPE_NAME(name)                                                 \
  HAZEL_VARLINE(::hazel::trace::Timer timer)                                   \
  (name)

// Swap `__PRETTY_FUNCTION__` with other macro, if the compiler doesn't support
// it
#define TIMER_SCOPE()                                                          \
  HAZEL_VARLINE(::hazel::trace::Timer timer)                                   \
  (__PRETTY_FUNCTION__)

struct timer_data {
  const char *name;
  float duration;
};

class Timer {
public:
  Timer(const char *name) : name(name), stopped(false) {
    this->start_time = std::chrono::high_resolution_clock::now();
  }

  ~Timer() {
    if (!this->stopped) {
      this->stop();
    }
  }

  void stop() {
    this->stopped = true;
    auto end_time = std::chrono::high_resolution_clock::now();
    long long start = std::chrono::time_point_cast<std::chrono::microseconds>(
                          this->start_time)
                          .time_since_epoch()
                          .count();
    long long end =
        std::chrono::time_point_cast<std::chrono::microseconds>(end_time)
            .time_since_epoch()
            .count();
    ::hazel::trace::Profiler::get()->write_session(
        {this->name, start, end, std::this_thread::get_id()});
  }

private:
  const char *name;
  bool stopped;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};
} // namespace hazel::trace