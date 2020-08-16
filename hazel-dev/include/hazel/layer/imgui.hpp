#pragma once

#include <hazel/layer/layer.hpp>

namespace hazel::layer {
class ImGui : public Layer {
public:
  ImGui();

  virtual void on_attach() override;

  virtual void on_detach() override;

  void begin();

  void end();

private:
  float time = 0.0f;
};
} // namespace hazel::layer
