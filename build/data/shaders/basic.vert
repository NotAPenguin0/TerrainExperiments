#version 460 core

layout(location = 0) in vec2 iPos;
layout(location = 1) in vec2 iTexCoords;

layout(location = 0) uniform mat4 projection_view;
layout(location = 1) uniform mat4 model;

layout(location = 2) uniform sampler2D height_map;

out vec2 TexCoords;

void main() {
    TexCoords = iTexCoords;
    float height = texture(height_map, TexCoords).r;
    gl_Position = projection_view * model * vec4(iPos.x, iPos.y, height, 1.0);
}