#include <hazel/camera/orthographic.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace hazel::camera
{
    Orthographic::Orthographic(float left, float right, float bottom, float top)
        : projection_matrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
          view_matrix(1.0f)
    {
        this->projection_view_matrix = this->projection_matrix * this->view_matrix;
    }

    void Orthographic::set_projection(float left, float right, float bottom, float top)
    {
        this->projection_matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        this->projection_view_matrix = this->projection_matrix * this->view_matrix;
    }

    void Orthographic::set_position(const glm::vec3 &pos)
    {
        this->current_position = pos;
        this->recalculate_matrix();
    }

    const glm::vec3 &Orthographic::get_position() const
    {
        return this->current_position;
    }

    void Orthographic::set_rotation(const float &rot)
    {
        this->current_rotation = rot;
        this->recalculate_matrix();
    }

    const float &Orthographic::get_rotation() const
    {
        return this->current_rotation;
    }

    const glm::mat4 &Orthographic::get_projection_view_matrix() const
    {
        return this->projection_view_matrix;
    }

    const glm::mat4 &Orthographic::get_projection_matrix() const
    {
        return this->projection_matrix;
    }

    const glm::mat4 &Orthographic::get_view_matrix() const
    {
        return this->view_matrix;
    }

    void Orthographic::recalculate_matrix()
    {
        glm::mat4 identity = glm::mat4(1.0f);

        auto translation = glm::translate(identity, this->current_position);

        auto z_axis = glm::vec3(0, 0, 1);
        auto rotation = glm::rotate(identity, glm::radians(this->current_rotation), z_axis);

        glm::mat4 transformation;
        if (this->translation_rotation == TRANSLATION_ROTATION::ROTATE_BEFORE_TRANSLATE)
        {
            transformation = translation * rotation;
        }
        else
        {
            transformation = rotation * translation;
        }

        // `transformation` is the object transformation, `view_matrix` is the camera's.
        // When the camera moves left, the object moves right, so it's always inverted.
        this->view_matrix = glm::inverse(transformation);

        this->projection_view_matrix = this->projection_matrix * this->view_matrix;
    }
} // namespace hazel::camera