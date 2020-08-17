#pragma once

#include <hazel/core/core.hpp>

struct profiler_data {
  const char *name;
  long long start, end;
  std::thread::id thread_id;
};

class Profiler {
public:
  Profiler() : profiler_count(0) {}

  void begin_session(const char *name,
                     const std::string &filepath = "profiler-results.json") {
    this->output_stream.open(filepath);
    this->write_header();
    this->config.name = name;
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

    std::string name = data.name;
    std::replace(name.begin(), name.end(), '"', '\"');

    this->output_stream << R"({"cat": "function", "ph": "X", "pid": 0, )";
    this->output_stream << "\"dur\": " << (data.end - data.start) << ", ";
    this->output_stream << "\"name\":" << '\"' << data.name << '\"' << ", ";
    this->output_stream << "\"tid\":" << data.thread_id << ", ";
    this->output_stream << "\"ts\":" << data.start;
    this->output_stream << "}";

    this->output_stream.flush();

    this->profiler_count++;
  }

  void write_header() {
    this->output_stream << R"({"otherData": {}, "traceEvents": [)";
    this->output_stream.flush();
  }

  void write_footer() {
    this->output_stream << R"(]})";
    this->output_stream.flush();
  }

private:
  struct profiler_config {
    const char *name;
  } config;
  unsigned int profiler_count;
  std::ofstream output_stream;
};

static hazel::core::Ref<Profiler> profiler =
    hazel::core::create_ref<Profiler>();

#define TIMER_SCOPE_NAME(name, into)                                           \
  Timer timer##__LINE__(name, [&](timer_data timer) { into.push_back(timer); })

#define TIMER_SCOPE(into)                                                      \
  Timer timer##__LINE__(__FUNCTION__,                                          \
                        [&](timer_data timer) { into.push_back(timer); })

struct timer_data {
  const char *name;
  float duration;
};

template <typename Fn> class Timer {
public:
  Timer(const char *name, Fn &&callback)
      : name(name), stopped(false), callback(callback) {
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
    this->callback({this->name, (float)(end - start) * 0.001f});
    profiler->write_session(
        {this->name, start, end, std::this_thread::get_id()});
  }

private:
  const char *name;
  bool stopped;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  Fn callback;
};