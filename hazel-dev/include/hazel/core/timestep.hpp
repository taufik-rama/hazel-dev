#pragma once

namespace hazel::core
{
    class Timestep
    {
    public:
        Timestep(float time = 0.0f) : time(time) {}

        float get_second() const { return this->time; }

        float get_millisecond() const { return this->time * 1000.0f; }

        operator float() const { return this->time; }

    private:
        float time;
    };
} // namespace hazel::core