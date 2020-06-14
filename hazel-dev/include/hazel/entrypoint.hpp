#pragma once

#include <hazel/application.hpp>
#include <hazel/core.hpp>
#include <hazel/meta.hpp>
#include <hazel/window.hpp>

#include <hazel/event/app.hpp>
#include <hazel/event/event.hpp>
#include <hazel/event/key.hpp>
#include <hazel/event/mouse.hpp>
#include <hazel/event/window.hpp>

#include <hazel/platform/linux/window.hpp>

extern hazel::Application *hazel::create_application();
