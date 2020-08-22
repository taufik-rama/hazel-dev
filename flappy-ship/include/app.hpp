#pragma once

#include <hazel-api.hpp>

#include <layer.hpp>

class App : public hazel::core::Application {
public:
  App();
  ~App();

private:
  hazel::core::Ref<hazel::layer::Layer> main_layer;
};
