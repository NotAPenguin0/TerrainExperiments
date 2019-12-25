#version 460 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iColor;

out vec3 Color;

void main() {
    Color = iColor;
    gl_Position = vec4(iPos, 1.0);
}