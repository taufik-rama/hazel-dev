#pragma once

#include <hazel-api.hpp>

#define PROFILE_SCOPE(name, into)                                              \
  Timer timer__LINE__(name,                                                    \
                      [&](profiler_t profiler) { into.push_back(profiler); })

struct profiler_t {
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
  }

private:
  const char *name;
  bool stopped;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  Fn callback;
};