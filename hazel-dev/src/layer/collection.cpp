#include <hazel/layer/collection.hpp>

namespace hazel::layer {
void Collection::add(hazel::core::Ref<Layer> layer) {
  this->layers.emplace(this->layers.begin() + this->current_layer, layer);
  this->current_layer++;
}

void Collection::remove(hazel::core::Ref<Layer> layer) {
  auto it = std::find(this->layers.begin(), this->layers.end(), layer);
  if (it != this->layers.end()) {
    this->layers.erase(it);
    this->current_layer--;
  }
}

void Collection::add_overlay(hazel::core::Ref<Layer> layer) {
  this->layers.emplace_back(layer);
}

void Collection::remove_overlay(hazel::core::Ref<Layer> layer) {
  auto it = std::find(this->layers.begin(), this->layers.end(), layer);
  if (it != this->layers.end()) {
    this->layers.erase(it);
  }
}
} // namespace hazel::layer