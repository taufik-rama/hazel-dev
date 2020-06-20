#pragma once

#include <hazel/core.hpp>
#include <hazel/layer/layer.hpp>

// ::TODO:: Change to `std::list`

namespace hazel::layer
{
    class Collection
    {
    public:
        Collection();

        ~Collection();

        void add(Layer *);

        void remove(Layer *);

        void add_overlay(Layer *);

        void remove_overlay(Layer *);

        std::vector<Layer *>::iterator begin() { return this->layers.begin(); }

        std::vector<Layer *>::iterator end() { return this->layers.end(); }

    private:
        std::vector<Layer *> layers;
        std::vector<Layer *>::iterator current_layer;
    };
} // namespace hazel::layer