#pragma once

#include <hazel/core.hpp>
#include <hazel/logger.hpp>

#include <hazel/event/window.hpp>

namespace hazel
{
    class Application
    {
    public:
        Application();

        virtual ~Application();

        void Run();
    };

    Application *create_application();

} // namespace hazel
