#pragma once

#include <std-pch.hpp>

namespace hazel::core {
template <typename T> using Scope = std::unique_ptr<T>;

template <typename T> using Ref = std::shared_ptr<T>;
} // namespace hazel::core
