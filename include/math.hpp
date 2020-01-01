#ifndef TERRAINS_MATH_HPP_
#define TERRAINS_MATH_HPP_

constexpr float pi = 3.1415926535;

struct vec2 {
    float x, y;

    float magnitude() const;

    vec2 normalized() const;
};

float random(float min, float max);

vec2 random_vec2();

float lerp(float a0, float a1, float w);

float map_value(float value, float start1, float stop1, float start2, float stop2);

#endif