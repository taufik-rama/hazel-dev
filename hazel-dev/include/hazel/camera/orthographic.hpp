#pragma once

#include <glm/glm.hpp>

namespace hazel::camera
{
    class Orthographic
    {
    public:
        Orthographic(float left, float right, float bottom, float top);

        void set_position(const glm::vec3 &pos);

        const glm::vec3 &get_position() const;

        void set_rotation(const float &rot);

        const float &get_rotation() const;

        const glm::mat4 &get_projection_view_matrix() const;

        const glm::mat4 &get_projection_matrix() const;

        const glm::mat4 &get_view_matrix() const;

    private:
        void recalculate_matrix();

    private:
        glm::mat4 projection_view_matrix, projection_matrix, view_matrix;
        glm::vec3 current_position = {0.0f, 0.0f, 0.0f};
        float current_rotation = 0.0f;
    };
} // namespace hazel::camera
