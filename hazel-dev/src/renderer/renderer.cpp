#include <hazel/renderer/renderer.hpp>

#include <hazel/camera/orthographic.hpp>
#include <hazel/renderer/command.hpp>

namespace hazel::renderer
{
    Renderer::SceneData Renderer::scene_data;

    void Renderer::begin_scene(hazel::camera::Orthographic &camera)
    {
        scene_data.projection_view_matrix = camera.get_projection_view_matrix();
    }

    void Renderer::end_scene()
    {
    }

    void Renderer::submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &va)
    {
        shader->bind();
        shader->upload_uniform("u_ViewProjection", scene_data.projection_view_matrix);

        va->bind();
        va->get_index_buffer()->bind();
        Command::draw_index(va);
        va->unbind();
        va->get_index_buffer()->unbind();

        shader->unbind();
    }
} // namespace hazel::renderer