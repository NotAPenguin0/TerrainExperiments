#ifndef TERRAINS_NOISE_HPP_
#define TERRAINS_NOISE_HPP_

#include <vector>

#include "math.hpp"

class PerlinNoise {
public:
    PerlinNoise(size_t scale = 1);

    std::vector<unsigned char> get_buffer(size_t w, size_t h) const;
    void get_buffer(unsigned char* buffer, size_t w, size_t h) const;

    float value(float x, float y) const;

    struct gradient_grid {
        vec2 size;
        std::vector<vec2> gradients;

        gradient_grid(size_t w, size_t h);

        vec2& at(size_t x, size_t y);
        vec2 const& at(size_t x, size_t y) const;
    };
private:
    gradient_grid gradients;
    size_t scale;
};

#endif