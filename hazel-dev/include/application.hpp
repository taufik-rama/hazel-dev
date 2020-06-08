#pragma once

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
