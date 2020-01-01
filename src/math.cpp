#include "math.hpp"

#include <cmath>
#include <random>

float vec2::magnitude() const {
    return std::sqrt(x * x + y * y);
}

vec2 vec2::normalized() const {
    float mag = magnitude();
    return {x / mag, y / mag};
}

float random(float min, float max) {
    static std::mt19937 engine(std::random_device{}());

    std::uniform_real_distribution<float> distr(min, max);
    return distr(engine);
}

vec2 random_vec2() {
    float angle = random(0, 2 * pi);
    float x = std::cos(angle);
    float y = std::sin(angle);
    return {x, y};
}

float lerp(float a0, float a1, float w) {
    return a0 + w * (a1 - a0);
}

float map_value(float value, float start1, float stop1, float start2, float stop2) {
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}