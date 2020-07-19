#pragma once

#include <hazel/renderer/texture.hpp>

namespace hazel::platform::linux
{
    class Texture2D : public hazel::renderer::Texture2D
    {
    public:
        Texture2D(const std::string &);

        ~Texture2D();

        virtual unsigned int get_width() const override;

        virtual unsigned int get_height() const override;

        virtual void bind(unsigned int slot) const override;

        virtual void unbind(unsigned int slot) const override;

    private:
        std::string path;
        unsigned int width, height;
        unsigned int renderer_id;
    };
} // namespace hazel::platform::linux
