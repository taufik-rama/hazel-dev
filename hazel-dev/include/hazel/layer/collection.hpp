#pragma once

#include <hazel/core/core.hpp>
#include <hazel/layer/layer.hpp>

// ::TODO:: Change to `std::list`

namespace hazel::layer {
class Collection {
public:
  void add(hazel::core::Ref<Layer>);

  void remove(hazel::core::Ref<Layer>);

  void add_overlay(hazel::core::Ref<Layer>);

  void remove_overlay(hazel::core::Ref<Layer>);

  std::vector<hazel::core::Ref<Layer>>::iterator begin() {
    return this->layers.begin();
  }

  std::vector<hazel::core::Ref<Layer>>::iterator end() {
    return this->layers.end();
  }

private:
  std::vector<hazel::core::Ref<Layer>> layers;
  unsigned int current_layer = 0;
};
} // namespace hazel::layer